# Agenda

## Homeworks

- Be very careful reading the specification.
	The `which` argument is not what you think it is if you just look at the process data-structure in the kernel.
- First of three assignments in `xv6`.
	The next two are much more challenging and this is meant to "boot you up" into `xv6`.
- Anything else to mention here?

## `xv6` System Call Arguments

We've discussed `xv6` system calls and gone through all the code.
You've seen the `int` instruction that switches to mode 0, and `iret` which returns from mode 0 back to 1 (back to user-mode).
I'm not going to retrace the functionality here, and instead talk about all of the data involved.
We'll end with the `arg*` family of functions for retrieving system call arguments.

- data-structures and arguments
- ...and their relationship to the code
- how system calls store and reference this data
- `fetch*` & `arg*` functions

Remember:

```c
int func(int a, int b, int c);
...
func(0, 1, 2);
// 0 (argument a) is at the bottom of the stack (closest to esp)
// 1 (argument b) is at the second to the bottom of the stack
// 2 (argument c) is at the third to the bottom of the stack
```
