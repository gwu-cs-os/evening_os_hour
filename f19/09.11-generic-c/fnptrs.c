#include <stdio.h>
#include <malloc.h>
#include <assert.h>

struct sorter {
	int nitems, start, end;
	int data[];
};

struct sorter *
sorter_create(int n)
{
	return NULL;
}

void
sorter_destroy(struct sorter *s)
{

}

void
sorter_add(struct sorter *s, int val)
{

}

int
sorter_get(struct sorter *s)
{
	return 0;
}

void
main(void)
{
	struct sorter *s;
	int i;

	s = sorter_create(32);
	sorter_add(s, 3);
	sorter_add(s, 1);
	sorter_add(s, 2);
	for (i = 0 ; i < 3 ; i++) {
		printf("%d\n", sorter_get(s));
	}
}
