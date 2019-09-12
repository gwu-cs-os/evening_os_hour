/*
 * add the #ifndef and associated commands to prevent lib1.h from
 * being included two times
 */
#ifndef LIB1_H
#define LIB1_H

#include <lib2.h>

static inline void
lib1_fn1(void)
{
	lib2_fn2();

	return;
}

#endif	/* LIB1_H */
