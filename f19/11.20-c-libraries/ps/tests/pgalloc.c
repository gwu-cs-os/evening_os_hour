#include <stdio.h>
#include <assert.h>
#include <ps_pgalloc.h>

struct ps_freelist pages = PS_PGALLOC_STATICDEF(PS_PAGE_SIZE);

#define ITER 1024

/* memory that has been seen... */
void *seen[ITER] = {0};
/* ...and is currently allocated */
void *allocated[ITER] = {0};

void
page_add(void *p)
{
	int i;

	/* check for proper page recycling */
	for (i = 0 ; i < ITER ; i++) {
		if (seen[i] == p || !seen[i]) {
			seen[i] = p;
			break;
		}
	}
	assert(i < ITER);

	/* check for double allocations */
	for (i = 0 ; i < ITER ; i++) {
		assert(allocated[i] != p);
		if (!allocated[i]) {
			allocated[i] = p;
			break;
		}
	}
}

void
page_rem(void *p)
{
	int i;

	for (i = 0 ; i < ITER ; i++) {
		if (allocated[i] != p) continue;

		allocated[i] = NULL;
		break;
	}
}

void *ptrs[ITER];

int
main(void)
{
	int i, j;

	for (i = ITER-1 ; i >= 0 ; i--) {
		for (j = i ; j < ITER ; j++) {
			ptrs[j] = ps_pgalloc(&pages);
			assert(ptrs[j]);
			page_add(ptrs[j]);
		}
		for (j = i ; j < ITER ; j++) {
			page_rem(ptrs[j]);
			ps_pgfree(&pages, ptrs[j]);
			ptrs[j] = NULL;
		}
	}
	printf("Page allocator unit tests:  SUCCESS!\n");

	return 0;
}
