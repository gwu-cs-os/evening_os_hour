#include <stdio.h>

#define CACHE_LINE_SZ 64
#define PAGE_SZ 4096

struct cache_line {
	char bytes[CACHE_LINE_SZ];
} __attribute__((aligned(CACHE_LINE_SZ)));

struct cache_line lines[(1 << 20) / CACHE_LINE_SZ];

struct page {
	char bytes[PAGE_SZ];
} __attribute__((aligned(PAGE_SZ)));

struct page pages[(1 << 20) / PAGE_SZ];

struct real_ds {
	int a, b;
	char padding[64-2*sizeof(int)];
} __attribute__((aligned(PAGE_SZ)));

/*
 * Locality:
 *
 * 1. within a cache-line/page
 * 2. prefetching when iterating
 *
 * versus
 *
 * - iterating through a linked list?
 */

struct node {
	void *data;
	struct node *next;
};

int
contains_ll(struct node *ll, void *cmp)
{
	struct node *i;

	/* 3 cycles for L1 access for next since it was brought in the same cache-line  */
	for (i = ll; i; i = i->next) {
		__prefetch(i->next);
		/* 40 cycles for the data access if it was in L3 */
		if (cmp == i->data) return 1;
		/* lots of computation here will make the prefetch worthwhile... */
	}

	return 0;
}

int
contains_arr(void **data_arr, int sz, void *cmp)
{
	int i;

	/*
	 * void * = 4 bytes (32 bit system) -> 16 of these in a 64 byte cache-line
	 * First word on cache-line = 40 cycles, next 15 = 3 cycles
	 */
	for (i = 0; i < sz; i++) {
		if (data_arr[i] == cmp) return 1;
	}

	return 0;
}

/*
 * Scorecard for 16 item data-structure
 *
 * - Linked list 43 * 16 = 688
 * - Arrays 40 + 15 * 3 = 85
 */
