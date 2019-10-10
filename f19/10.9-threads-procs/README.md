# Schedule

## Class Business

### Thread operations and process operations in Linux

Core kernel data-structures:

- `struct mm_struct`
	- includes a "list" of `vm_area_struct`s
	- pgd reference
	- reference count
- `struct task_struct`
	- regs
	- mm -> mm_struct

- Calling `fork` in a `thread`

### Trapframe and context -- registers are uint...whaaa?

- Type sizes in C and use of pointers vs. ints

### argptr and threads -- the breakage

- TTCTTOU errors



## Nothing else today, sorry!

- That's all folks
