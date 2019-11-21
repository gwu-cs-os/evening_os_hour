/***
 * Copyright 2014-2015 by Gabriel Parmer.  All rights reserved.
 * Redistribution of this file is permitted under the BSD 2 clause license.
 *
 * Authors: Qi Wang, interwq@gmail.com, Gabriel Parmer, gparmer@gwu.edu, 2015
 *
 * History:
 * - Started as parsec.c and parsec.h by Qi.
 */

#include <ps_smr.h>


void __ps_timing_info_init(struct parsec *ps, ps_tsc_t now);
void __ps_smr_account_init(struct ps_smr_info *si);

int
__ps_smr_reclaim_batch(int batch, ps_tsc_t qsc, coreid_t curr, struct ps_qsc_list *ql, 
		       struct ps_mem *m, ps_free_fn_t ffn)
{
	int i;
	struct ps_mheader *a;
	/* Remove a batch worth of items from the qlist */
	for (i = 0 ; i < batch ; i++) {
		a = __ps_qsc_peek(ql);
		if (!a || a->tsc_free > qsc) break;

		a = __ps_qsc_dequeue(ql);
		assert(a && __ps_mhead_isfree(a));
		__ps_mhead_reset(a);
		ffn(m, __ps_mhead_mem(a), 0, curr);
	}
	return i;
}

size_t
ps_smr_nqueued(struct ps_mem *m)
{ return m->percore[ps_coreid()].smr_info.account.qmemcnt; }

void
ps_init(struct parsec *ps)
{
	ps_tsc_t now = ps_tsc();
	int i;

	assert(ps);
	memset(ps, 0, sizeof(struct parsec));

	ps->refcnt = 0;
	for (i = 0 ; i < PS_NUMCORES ; i++) {
		struct ps_quiescence_timing *t = &ps->timing_info[i].timing;

		t->time_in  = now;
		t->time_out = t->time_in + 1;
	}
	__ps_timing_info_init(ps, now);
}

struct parsec *
ps_alloc(void)
{
	struct parsec *ps = ps_plat_alloc(sizeof(struct parsec), ps_coreid());

	if (!ps) return NULL;
	ps_init(ps);

	return ps;
}

int
ps_free(struct parsec *ps)
{
	if (ps->refcnt > 0) return -1;
	ps_plat_free(ps, sizeof(struct parsec), ps_coreid());

	return 0;
}

void
__ps_memptr_init(struct ps_mem *m, struct parsec *ps)
{
        struct ps_mem_percore *pc = &m->percore[0];
	int i;

	assert(m && ps);
	for (i = 0 ; i < PS_NUMCORES ; i++) {
		__ps_smr_account_init(&pc[i].smr_info);
		pc[i].smr_info.ps         = ps;
	}
	ps->refcnt++;
}

int
__ps_memptr_delete(struct ps_mem *m)
{
        struct ps_mem_percore *pc = &m->percore[0];
	struct parsec         *ps = pc->smr_info.ps;
	int i;

	if (!ps) return 0;
	if (!ps_slabptr_isempty(m)) return -1;
	for (i = 0 ; i < PS_NUMCORES ; i++) {
		if (__ps_qsc_peek(&pc[i].smr_info.qsc_list)) return -1;
	}
	ps->refcnt--;
	/* TODO: actually delete it iff refcnt == 0 */

	return 0;
}
