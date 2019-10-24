#include <stdio.h>

#define CACHE_LINE_SZ 64
#define PAGE_SZ

struct cache_line {
	char bytes[CACHE_LINE_SZ];
} __attribute__((aligned(CACHE_LINE_SZ)));

struct cache_line lines[(1 << 20) / CACHE_LINE_SZ];

struct page {
	char bytes[PAGE_SZ];
} __attribute__((aligned(PAGE_SZ)));

struct page pages[(1 << 20) / PAGE_SZ];

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

	for (i = ll; i; i = i->next) {
		if (cmp == i->data) return 1;
	}

	return 0;
}

int
contains_arr(void **data_arr, int sz, void *cmp)
{
	int i;

	for (i = 0; i < sz; i++) {
		if (data_arr[i] == cmp) return 1;
	}

	return 0;
}
