# Schedule

## Containers and communication

See `containers_comms.svg`.

## Futexes and race conditions

Context: Systems hall of fame

Idea: mutex_lock and _unlock should not make system calls IF there is no contention.

How:

1. use cas to determine at user-level if someone holds the lock
	...take it if they do not
	...and set a bit saying the lock is contented if they are
2. call the kernel to block if the lock is held by someone else
	...and call the kernel to wake any threads that are blocked when the lock is released

## Differentiating between processes

- Do processes in different containers have overlapping pids?
- how can we differentiate them? should have container ids?
