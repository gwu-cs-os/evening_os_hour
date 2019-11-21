/***
 * Copyright 2015 by Gabriel Parmer.  All rights reserved.
 * Redistribution of this file is permitted under the BSD 2 clause license.
 *
 * Authors: Gabriel Parmer, gparmer@gwu.edu, 2015
 */

#ifndef PS_OS_H
#define PS_OS_H

#include <assert.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

/* Linux library */
#include <pthread.h>

/* useful abstractions on Linux */
void set_prio(void);
void thd_set_affinity(pthread_t tid, int id);
void meas_barrier(int ncores);

#include <ps_config.h>
#include <ps_plat.h>

/* Default allocation and deallocation functions */
static inline void *
ps_plat_alloc(size_t sz, coreid_t coreid)
{
	void *m;
	(void)coreid;

	m = aligned_alloc(PS_PAGE_SIZE, sz);
	assert(m);
	memset(m, 0, sz);

	return m;
	/* mmap(0, sz, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, (size_t)0); */
}

static inline void
ps_plat_free(void *s, size_t sz, coreid_t coreid)
{
	(void)coreid; (void)sz;
	free(s);
	/* munmap(s, sz); */
}

/*
 * These functions are ambiguously os vs. x86 as the locality API
 * requires that the underlying OS set up the locality information
 * readable with rdtscp correctly.  Arbitrary decision to put it here
 * for now.
 */
static inline ps_tsc_t
ps_tsc_locality(coreid_t *coreid, localityid_t *numaid)
{
	unsigned long a, d, c;

	__asm__ __volatile__("rdtscp" : "=a" (a), "=d" (d), "=c" (c) : : );
	*coreid = c & 0xFFF; 	/* lower 12 bits in Linux = coreid */
	*numaid = c >> 12; 	/* next 8 = socket/numa id */

	return ((u64_t)d << 32) | (u64_t)a;
}

static inline unsigned int
ps_coreid(void)
{
	coreid_t coreid, numaid;

	if (PS_NUMCORES == 1) return 0;
	ps_tsc_locality(&coreid, &numaid);

	return coreid;
}


#endif	/* PS_OS_H */
