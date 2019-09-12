#pragma once

// extern int x;

int get_x(void);
void set_x(int _x);

static inline int
times2(int x)
{
	return x * 2;
}
