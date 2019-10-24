# Schedule

## Office Hours

- thread homework
- heap sharing
- synchronization

- midterm
	- crib sheet
	- studying


## Atomic Elements in Operating Systems

See post @ https://www2.seas.gwu.edu/~gparmer/posts/2016-05-07-operating-systems-atoms.html

This post covers the fundamental mechanisms provided by the hardware, and asks a number of critical thinking questions throughout.
In this segment, I'll discuss the general post, and the critical thinking questions.

### Locality

- Cache-line aligned.
- Page aligned.

### Costs

- Direct costs
- Indirect costs

### Dual mode

- register saving (trapasm.S)
- stacks and saving registers

### Page Tables and Isolation

- cr3
- bits to protect the kernel (main.c & mmu.h)
- measuring indirect costs of dual-mode and VAS switching

### Compounds

- Thread switch
- IPC Sync and Async
