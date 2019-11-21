/***
 * Copyright 2017 by Gabriel Parmer.  All rights reserved.
 * Redistribution of this file is permitted under the BSD 2 clause license.
 *
 * A simple page allocator and free list.  Dumbest possible memory
 * allocation.  Useful instead of a buddy allocator when you know that
 * all allocations will be of a specific size.
 */

#ifndef PS_PGALLOC_H
#define PS_PGALLOC_H

#include <ps_plat.h>

struct ps_freelist {
	size_t pgsize;
	struct ps_freelist *next;
};

#define PS_PGALLOC_STATICDEF(page_size) { .pgsize = page_size, .next = NULL }
#define PS_PGALLOC_INITDEF(page_size) (struct ps_freelist)PS_PGALLOC_STATICDEF(page_size)

static inline void
ps_pgalloc_init(struct ps_freelist *fl, size_t pgsize)
{ *fl = PS_PGALLOC_INITDEF(pgsize); }

/*
 * We have to assume that all previously allocated memory is freed...
 * Not only is this required for correctness, it is used here to avoid
 * the need for atomic instructions.
 */
static inline void
ps_pgalloc_destroy(struct ps_freelist *fl)
{
	struct ps_freelist *l, *n;

	for (l = fl->next ; l ; l = n) {
		n = l->next;
		ps_plat_free(l, fl->pgsize, ps_coreid());
	}
}

static inline void *
ps_pgalloc(struct ps_freelist *fl)
{
	void *a = NULL;
	struct ps_freelist *n;

retry:
	n = ps_load(&fl->next);
	if (n) {
		/* FIXME: ABA */
		if (!ps_cas((unsigned long *)&fl->next, (unsigned long)n, (unsigned long)ps_load(&n->next))) goto retry;

		a       = n;
		n->next = NULL;
	} else {
		a = ps_plat_alloc(fl->pgsize, ps_coreid());
	}

	return a;
}

static inline void
ps_pgfree(struct ps_freelist *fl, void *p)
{
	struct ps_freelist *l, *n;

	l = (struct ps_freelist *)p;
retry:
	l->next = n = fl->next;
	if (!ps_cas((unsigned long *)&fl->next, (unsigned long)n, (unsigned long)l)) goto retry;

	return;
}

#endif	/* PS_PGALLOC_H */
