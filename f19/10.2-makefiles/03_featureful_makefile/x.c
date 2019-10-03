#include <stdio.h>

#include <z.h>

int
xfn(void)
{
	return yfn(1);
}

int
main(void)
{
	printf("%d\n", xfn());

	return 0;
}
