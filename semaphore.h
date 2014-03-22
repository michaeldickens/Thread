/* 
 * 
 * semaphore.h
 * -----------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-22
 * 
 * Semaphore implementation built on top of C multithreading library.
 * 
 */

#include <pthread.h>

struct semaphore;
typedef struct semaphore semaphore_t;

struct semaphore {
	size_t counter;
	pthread_mutex_t lock;
	pthread_cond_t cond;
};

int semaphore_init(semaphore_t *semaphore, size_t counter);

/*
 * ??? How to describe this?
 */
int semaphore_signal(semaphore_t *semaphore);

/*
 * ??? How to describe this?
 */
int semaphore_wait(semaphore_t *semaphore);
