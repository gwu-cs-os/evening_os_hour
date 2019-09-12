#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>

struct sorter {
	int nitems, start, end;
	void **data;
};

struct sorter *
sorter_create(int n)
{
	struct sorter *s;
	int i;
	void **data;

	s = malloc(sizeof(struct sorter));
	if (s == NULL) return NULL;

	data = malloc(sizeof(void *) * n);
	if (data == NULL) {
		/*
		 * read the Composite style guide for a way to make
		 * this code generic and shared with other error paths
		 * using gotos
		 */
		free(s);

		return NULL;
	}
	for (i = 0 ; i < n ; i++) {
		data[i] = NULL;
	}

	*s = (struct sorter){
		.nitems = n,
		.start  = 0,
		.end    = 0,
		.data   = data
	};

	return s;
}

int
sorter_destroy(struct sorter *s)
{
	assert(s);
	/* TODO: do this function */

	return -ENOTSUP;
}

/*
 * Add value to the DS, but ensure that it is added in a sorted
 * location.
 */
void
sorter_add(struct sorter *s, void *val, int (*cmp_fn)(void *a, void *b))
{
	void **arr;
	int start, end, i;

	assert(s);
	/* I'm assuming that no gets have been called here */
	assert(s->start == 0);

	/* add error checking...I'm bad */
	arr   = s->data;
	start = s->start;
	end   = s->end + 1; 	/* new end *after* adding */

	for (i = 0 ; i < end - start ; i++) {
		void *tmp = arr[i];

		if (tmp != NULL && cmp_fn(tmp, val)) continue;
		arr[i] = val;
		val = tmp;
	}
	/* TODO: add the ability to add *after* we've called get */

	s->end = end;

	return;
}

void *
sorter_get(struct sorter *s)
{
	void *val;

	assert(s);

	val = s->data[s->start];
	s->start++;

	return val;
}

int
intptr_cmp_fn(void *a, void *b)
{
	int ai, bi;

	assert(a && b);

	ai = *(int *)a;
	bi = *(int *)b;

	return ai < bi;
}

void
main(void)
{
	struct sorter *s;
	int i;
	int all_the_data[3] = {3, 1, 2};

	s = sorter_create(32);
	for (i = 0 ; i < 3 ; i++) {
		sorter_add(s, &all_the_data[i], intptr_cmp_fn);
	}
	for (i = 0 ; i < 3 ; i++) {
		printf("%d\n", *(int *)sorter_get(s));
	}
}
