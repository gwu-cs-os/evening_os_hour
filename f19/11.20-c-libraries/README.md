# Schedule

## Office hours

Project

- Lock and Shm:

	1. track who has access to which lock/shm
	2. track the lock/shm

- Try and work *with* the system as a whole.
	If there is a system call that does some aspect of what you want to do, read and understand that code.

- Once you have used `setroot`, note that it should limit `cd ..`, but also any system call that takes a path as an argument (e.g. `open`).
	Paths might be relative: `open("ls",...)`, but some are going absolute: `open("/bin/ls", ...)`

HW6

```
memory        : --------------------
with freelists: f-------f-----f-----
with allocs   : f--haaaaf---haf---ha
with a malloc : f--haaaahaaahaf---ha
with a free   : f--f----haaahaf---ha
```

- Allocate a new page: partition properly

	- if there currently an allocation for 1800 bytes, you're looking for a chunk that is 2048 bytes...if you need to allocate a new page, you'll immediately separate the page into two 2048 chunks, populate the freelist with them

## Cool C libraries (that echo aspects of the class)

- [jsmn](https://github.com/zserge/jsmn) - json parser
- [ck](https://github.com/concurrencykit/ck) - concurrency kit (written by a GWU alum) including spinlock implementations, lock- and wait-free hash-tables, etc...
- [parsec](https://github.com/gwsystems/ps) - scalable data-structures including a lock-free slab allocator
- Anything that [Ian Piumarta](https://www.piumarta.com/software/) has written
