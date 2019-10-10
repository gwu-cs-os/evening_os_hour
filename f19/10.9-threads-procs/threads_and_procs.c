#include <pthread.h>

pthread_mutex mut = PTHREAD_MUTEX_INIT;
int count = 0;

void *
fn(void *data)
{

	while (1) {
		pthread_mutex_lock(&mut);
		count++;
		pthread_mutex_unlock(&mut);
		if (count == 42) {
			pid_t pid = fork();
			/* ... */
		}
		/*
		 * Interesting interleaving:
		 * thread 1: inside the critical section
		 * thread 2: calling fork
		 *
		 * fork will "fork" the current thread calling fork,
		 * but not any other threads in the process. But who
		 * holds the lock in the child process that only
		 * "fork"ed the thread not holding the lock.
		 *
		 * what if the other thread that is not forked is in
		 * the process of modifying a data-structure? What
		 * state is that data-structure in the child process?
		 */
	}
}

void
main(void)
{
	pthread_t pt1, pt2;

	pt1 = pthread_create(fn, NULL);
	pt2 = pthread_create(fn, NULL);
	pthread_join(&pt1);
	pthread_join(&pt2);
}
