# Schedule

All project, all the time.

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

What is the race condition here?

1. Thread A does mutex_lock -> (owned = 1, contested = 0)
2. Preemption -> thread B
3. Thread B mutex_lock -> (owned = 1, contested = 1)
4. Thread B is preemption -> thread A
5. Thread A mutex_release -> (owned = 0, contested = 0)
6. Thread A calls kernel to wakeup all blocked threads on the mutex
   -> kernel says "OK there are no threads to wakeup, so...thanks for that"
7. switch back to thread B
8. continue executing Thread B that is in the process of calling the kernel to block
   -> go into the kernel and block....forever more

## Differentiating between processes

- Do processes in different containers have overlapping pids?
- how can we differentiate them? should have container ids?

## Dirlookup scope

- does it search just the cwd? or all subdirs?
- what is the scope and FS within a container?
