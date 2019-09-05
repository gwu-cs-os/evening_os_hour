#include <stdio.h>

#include <lib1.h>
#include <lib2.h>

#include <main.h>

static int x = 42;

extern void obj_fn(void);

int
get_x(void)
{
	return x;
}

void
set_x(int _x)
{
	x = _x;
}

void
main(void)
{
	lib1_fn1();
	lib2_fn1();

	obj_fn();
	x /= 2;

	printf("%d\n", x);
}
