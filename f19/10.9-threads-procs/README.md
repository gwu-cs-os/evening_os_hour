# Schedule

## Class Business

### Thread operations and process operations in Linux

Core kernel data-structures:

- `struct mm_struct`
	- includes a "list" of `struct vm_area_struct`s - all of the contiguous memory regions within a process (within a VAS), each of these has the vas and the size, properties including rwx, ...
	- `pgd` reference
	- reference count - count of the threads are executing in the VAS
- `struct task_struct`
	- regs
	- mm -> mm_struct

- google either of the structures

- Calling `fork` in a `thread`
	- see the .c file in this dir for an example

### Trapframe and context -- registers are uint...whaaa?

- Type sizes in C and use of pointers vs. ints

- uint = unsigned int = 32 bits on a 32 bit machine
- void * = 32 bits (sizeof(void *) * 8)
- if you were to port xv6 to 64 bit machine
    - uint = 32 bits
	- unsigned long = 64 bits on 64 bit machine, 32 bits on 32 bit machine
	- void * = 64 bits
	- What I would do is `typedef unsigned long reg_t`, or `word_t`

### argptr and threads -- the breakage

1. `thread_a` allocates a new thread (yeah, gwthreads!) `thread_b`
2. `thread_a` calls `fstat` passing a pointer to `sbrk(grow)` (similar to `malloc`ed memory)

```c
struct fstat *fs = sbrk(4096);
assert(fs);
fstat(fd, fs);
```

3. *before* kernel has completed fstat system call logic, and *after* `argptr` has claimed "yes, the fstat structure passed in from user-level is a-OK (within valid memory won't cause a kernel fault to access)."
	- switch to `thread_b` and that thread calls `sbrk(-4096)`.
	- This has a disastrous effect. the kernel can deference the fstat structure, but that structure no long exists in memory.

- TTCTTU -- Time to check to time to use errors
	- time to check = argptr (sanitize the argument and *check* that is OK to use)
	- time to use = deferences the structure

- Linux requires that you *copy* the structure in the kernel
	- But what about ttcttu errors *while* copying the structure
	- does not `memcpy`, instead it uses `copy_from_user` and `copy_to_user`




## Nothing else today, sorry!

- That's all folks
