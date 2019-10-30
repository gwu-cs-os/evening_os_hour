# Midterm Questions!

## General

Can you give an example of the kind of questions that will be on the test (applying concepts from one topic to another to form a question)?

*Suggested answer*: Do asynchronous and synchronous operations preserve bounded wait? Why or why not?

## System Structure

### Costs

can we briefly go over overhead concepts:

1. context switch between threads
2. traps/interrupts
3. switching VAS
4. IPC
5. device I/O

*Suggested answer*:

been thinking about the different overheads and am trying to rank them in terms of (relatively) best to worst

here's my tentative list (pls correct it)

1. context switch between threads
2. traps/interrupts
3. switching VAS
4. IPC
5. device I/O

### Concepts

Can we review some of system structure concepts from lecture 3? Specifically, motivations/pros/cons for different system structures and implementations including:

- Monolithic vs. Microkernel
- Virtualization (VMs)
- Containers

Additionally, should we understand how later concepts would function in some of these alternate (perhaps worse) system structures? (e.g. how multi-threading/synchronization might happen in a microkernel system)

### Definitions

In regards to VMs, is the hypervisor and the kernel the same thing? Specifically, are there "two kernels" with one handeling the syscalls from VM and the other handeling syscalls from the host device or are they the same?

## Sync

-been trying to implement condition vars for the r/w code... I know you asked us to do it ourselves but have no idea if I did it correctly....could you go over this?

-solution 2 for dining philosophers... was looking at the slide and there isn't context for the functions so I'm not sure what the issue is could you go over it?

*Answer*: there is no solution 1, 2, and 3, there are slide 1, 2, and 3.
Slide 2 is providing context for the condition-variable solution.

-is deadlock possible w/async communication between thds (I'm leaning towards no but who knows??)

## Scheduling

-does the scheduler know how many cores the computer has?

## Threading

For the Facebook server problem, I am having a hard time with the concepts. For the different scenario, this is what I think.

Facebook webserver:
a) Single thread
  Throughput: Not great because for each client requests, it performs all calculations and reads from the disk
  Reliability:  No. Because if a calculation fails then no client request will be calculated
  Parallelism: No. Because we only have one thread performing the calculation

b) Multi process server
  Throughput: Not too great because each worker compute and do disk i/O and the access to disk i/o increases throughput. Also, if the IPC is done via memory sharing it will not increase the throughput as will message passing.
  Reliability: Yes, because if a process fails it will not affect the others
  Parallelism: Yes, different processes are doing different operations


c) Multi-threaded process
  Throughput: Better, because only one thread will do the disk i/o at a time.
  Reliability:  Not reliable because if a thread does a disk i/o and there is a problem, then no other thread will be blocked.
  Parallelism: yes because many threads will compute different operations.
