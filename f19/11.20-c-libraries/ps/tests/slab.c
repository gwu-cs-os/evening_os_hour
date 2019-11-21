/***
 * Copyright 2015 by Gabriel Parmer.  All rights reserved.
 * This file is dual licensed under the BSD 2 clause license.
 *
 * Authors: Gabriel Parmer, gparmer@gwu.edu, 2015
 */

#include <stdio.h>
#include <stdlib.h>

#include <ps_slab.h>

#define SMALLSZ 1
#define LARGESZ 8000

struct small {
	char x[SMALLSZ];
};

struct larger {
	char x[LARGESZ];
};

PS_SLAB_CREATE_DEF(s, sizeof(struct small))
PS_SLAB_CREATE(l, sizeof(struct larger), PS_PAGE_SIZE * 128)
PS_SLAB_CREATE(hextern, sizeof(struct larger), PS_PAGE_SIZE * 128)

#define ITER       (1024)
#define SMALLCHUNK 2
#define LARGECHUNK 32

/* These are meant to be disassembled and inspected, to validate inlining/optimization */
void *
disassemble_alloc()
{ return ps_slab_alloc_l(); }
void
disassemble_free(void *m)
{ ps_slab_free_l(m); }

void
mark(char *c, int sz, char val)
{
	int i;

	for (i = 0 ; i < sz ; i++) c[i] = val;
}

void
chk(char *c, int sz, char val)
{
	int i;

	for (i = 0 ; i < sz ; i++) assert(c[i] == val);
}

struct small  *s[ITER];
struct larger *l[ITER];

#define FREE_BATCH 64
#define RB_SZ   ((PS_NUMCORES-1)*FREE_BATCH)
#define RB_ITER (RB_SZ * 1024)

void * volatile ring_buffer[RB_SZ] PS_ALIGNED;

unsigned long long free_tsc, alloc_tsc;

void
consumer(void)
{
	struct small *s;
	unsigned long i;
	unsigned long long start, end, tot = 0;

	meas_barrier(2);

	for (i = 0 ; i < RB_ITER ; i++) {
		unsigned long off = i % RB_SZ;

		while (!ring_buffer[off]) ;
		s = ring_buffer[off];
		ring_buffer[off] = NULL;

		start = ps_tsc();
		ps_slab_free_s(s);
		end = ps_tsc();
		tot += end-start;
	}
	free_tsc = tot / RB_ITER;

	meas_barrier(2);
}

void
producer(void)
{
	struct small *s;
	unsigned long i;
	unsigned long long start, end, tot = 0;

	meas_barrier(2);

	for (i = 0 ; i < RB_ITER ; i++) {
		unsigned long off = i % RB_SZ;

		while (ring_buffer[off]) ;

		start = ps_tsc();
		s = ps_slab_alloc_s();
		end = ps_tsc();
		tot += end-start;

		assert(s);
		ring_buffer[off] = s;
	}
	alloc_tsc = tot / RB_ITER;

	meas_barrier(2);
}

void *
child_fn(void *d)
{
	(void)d;

	thd_set_affinity(pthread_self(), 1);
	consumer();

	return NULL;
}

void
test_remote_frees(void)
{
	pthread_t child;

	printf("Starting test for remote frees\n");

	if (pthread_create(&child, 0, child_fn, NULL)) {
		perror("pthread create of child\n");
		exit(-1);
	}

	producer();

	pthread_join(child, NULL);
	printf("Remote allocations take %lld, remote frees %lld (unadjusted for tsc)\n", alloc_tsc, free_tsc);
}

#define STATS_REPORT_THD 2
#define REMOTE_FREE_ITER (100000)

unsigned long cost[REMOTE_FREE_ITER]  PS_ALIGNED;
unsigned long alloc[REMOTE_FREE_ITER] PS_ALIGNED;
__thread int thd_local_id;

static inline int
cmpfunc(const void * a, const void * b)
{ return (*(unsigned long*)b) - (*(unsigned long*)a); }

static inline void
out_latency(unsigned long *re, int num, char *label)
{
	int i;
	unsigned long long sum = 0;

	for (i = 0; i < num; i++) sum += (unsigned long long)re[i];
	qsort(re, num, sizeof(unsigned long), cmpfunc);
	printf("thd %d %s tot %d avg %llu 99.9 %lu 99 %lu min %lu max %lu\n", thd_local_id,
	       label, num, sum/num, re[num/1000], re[num/100], re[num-1], re[0]);
}

void
mt_consumer(void)
{
	char *s, *h;
	int id = thd_local_id, k = 0;
	long b, e, i;
	unsigned long long start, end;

	b = (id-1)*FREE_BATCH;
	e = id*FREE_BATCH;
	meas_barrier(PS_NUMCORES);

c_begin:
	for (i = b; i < e; i++) {
		while (!ring_buffer[i]) ;
		s = (char *)ring_buffer[i];
		if (s == (void *)-1) goto c_end;

		ring_buffer[i] = NULL;
		assert(i == ((int *)s)[0]);
		h = s-sizeof(struct ps_mheader);
		h[0] = 0;
		ps_mem_fence();

		start = ps_tsc();
		ps_slab_free_s(s);
		end = ps_tsc();
		if (id == STATS_REPORT_THD && k < REMOTE_FREE_ITER) cost[k++] = end-start;
	}
	goto c_begin;

c_end:
	if (id == STATS_REPORT_THD) out_latency(cost, k, "remote_free");
	meas_barrier(PS_NUMCORES);
}

void
mt_producer(void)
{
	void *s;
	unsigned long i, k = 0, b = 0;
	unsigned long long start, end;

	meas_barrier(PS_NUMCORES);

p_begin:
	for (i = b; i < RB_SZ; i += (PS_NUMCORES-1)) {
		if (ring_buffer[i]) continue;
		start = ps_tsc();
		s = ps_slab_alloc_s();
		end = ps_tsc();
		assert(s);

		((int *)s)[0] = i;
		ps_mem_fence();
		ring_buffer[i] = s;
		if (k < REMOTE_FREE_ITER) alloc[k] = end-start;
		if ((++k) == (PS_NUMCORES-1)*REMOTE_FREE_ITER) goto p_end;
	}
	b = (b+1) % FREE_BATCH;
	goto p_begin;

p_end:
	for(i=0; i<RB_SZ; i++) ring_buffer[i] = (void *)-1;
	out_latency(alloc, REMOTE_FREE_ITER, "alloc");
	meas_barrier(PS_NUMCORES);
}

void *
child_mt_fn(void *d)
{
	(void)d;

	thd_local_id = (int)(long)d;
	thd_set_affinity(pthread_self(), thd_local_id);
	mt_consumer();

	return NULL;
}

void
test_remote_frees_multi_thd(void)
{
	pthread_t child[PS_NUMCORES];
	int ret;
	long i, *s;

	printf("Starting test for multi-thread remote frees\n");
	for (i =0 ; i < RB_SZ; i++) {
		s = (long *)ps_slab_alloc_s();
		s[0] = i;
		ring_buffer[i] = (void *)s;
	}

	for (i = 1; i < PS_NUMCORES; i++) {
		ret = pthread_create(&child[i], 0, child_mt_fn, (void *)i);
		if (ret) {
			perror("pthread create of child\n");
			exit(-1);
		}
	}

	mt_producer();

	for (i = 1; i < PS_NUMCORES; i++) {
		pthread_join(child[i], NULL);
	}
}

void
test_correctness(void)
{
	int i, j;

	printf("Starting mark & check for increasing numbers of allocations.\n");
	for (i = 0 ; i < ITER ; i++) {
		l[i] = ps_slab_alloc_l();
		mark(l[i]->x, sizeof(struct larger), i);
		for (j = i+1 ; j < ITER ; j++) {
			l[j] = ps_slab_alloc_l();
			mark(l[j]->x, sizeof(struct larger), j);
		}
		for (j = i+1 ; j < ITER ; j++) {
			chk(l[j]->x, sizeof(struct larger), j);
			ps_slab_free_l(l[j]);
		}
	}
	for (i = 0 ; i < ITER ; i++) {
		assert(l[i]);
		chk(l[i]->x, sizeof(struct larger), i);
		ps_slab_free_l(l[i]);
	}
}

void
test_perf(void)
{
	int i, j;
	unsigned long long start, end;

	printf("Slabs:\n"
	       "\tsmall: objsz %lu, objmem %lu, nobj %lu\n"
	       "\tlarge: objsz %lu, objmem %lu, nobj %lu\n"
	       "\tlarge+nohead: objsz %lu, objmem %lu, nobj %lu\n",
	       (unsigned long)sizeof(struct small),  (unsigned long)ps_slab_objmem_s(), (unsigned long)ps_slab_nobjs_s(),
	       (unsigned long)sizeof(struct larger), (unsigned long)ps_slab_objmem_l(), (unsigned long)ps_slab_nobjs_l(),
	       (unsigned long)sizeof(struct larger), (unsigned long)ps_slab_objmem_hextern(), (unsigned long)ps_slab_nobjs_hextern());

	start = ps_tsc();
	for (j = 0 ; j < ITER ; j++) {
		for (i = 0 ; i < LARGECHUNK ; i++) s[i] = ps_slab_alloc_l();
		for (i = 0 ; i < LARGECHUNK ; i++) ps_slab_free_l(s[i]);
	}
	end = ps_tsc();
	end = (end-start)/(ITER*LARGECHUNK);
	printf("Average cost of large slab alloc+free: %lld\n", end);

	ps_slab_alloc_s();
	start = ps_tsc();
	for (j = 0 ; j < ITER ; j++) {
		for (i = 0 ; i < SMALLCHUNK ; i++) s[i] = ps_slab_alloc_s();
		for (i = 0 ; i < SMALLCHUNK ; i++) ps_slab_free_s(s[i]);
	}
	end = ps_tsc();
	end = (end-start)/(ITER*SMALLCHUNK);
	printf("Average cost of small slab alloc+free: %lld\n", end);

	ps_slab_alloc_hextern();
	start = ps_tsc();
	for (j = 0 ; j < ITER ; j++) {
		for (i = 0 ; i < LARGECHUNK ; i++) s[i] = ps_slab_alloc_hextern();
		for (i = 0 ; i < LARGECHUNK ; i++) ps_slab_free_hextern(s[i]);
	}
	end = ps_tsc();
	end = (end-start)/(ITER*LARGECHUNK);
	printf("Average cost of extern slab header, large slab alloc+free: %lld\n", end);
}

void
stats_print(struct ps_mem *m)
{
	struct ps_slab_stats s;
	int i;

	printf("Stats for slab @ %p\n", (void*)m);
	ps_slabptr_stats(m, &s);
	for (i = 0 ; i < PS_NUMCORES ; i++) {
		printf("\tcore %d, slabs %zd, partial slabs %zd, nfree %zd, nremote %zd\n",
		       i, s.percore[i].nslabs, s.percore[i].npartslabs, s.percore[i].nfree, s.percore[i].nremote);
	}
}

int
main(void)
{
	thd_set_affinity(pthread_self(), 0);

	test_perf();

	stats_print(&__ps_mem_l);
	stats_print(&__ps_mem_s);
	test_correctness();
	stats_print(&__ps_mem_l);
	test_remote_frees();
	stats_print(&__ps_mem_s);
	test_remote_frees_multi_thd();
	stats_print(&__ps_mem_s);

	return 0;
}
