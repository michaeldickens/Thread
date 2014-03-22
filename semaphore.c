/* 
 * 
 * semaphore.c
 * -----------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-22
 * 
 */

#include "semaphore.h"

int semaphore_init(semaphore_t *semaphore, size_t counter)
{
	semaphore->counter = counter;
	pthread_mutex_init(&semaphore->lock, NULL);
	pthread_cond_init(&semaphore->cond, NULL);
	return 0;
}

int semaphore_signal(semaphore_t *semaphore)
{
	pthread_mutex_lock(&semaphore->lock);
	++semaphore->counter;
	pthread_mutex_unlock(&semaphore->lock);
	pthread_cond_signal(&semaphore->cond);

	return 0;
}

int semaphore_wait(semaphore_t *semaphore)
{
	pthread_mutex_lock(&semaphore->lock);
	while (semaphore->counter == 0) {
		pthread_cond_wait(&semaphore->cond, &semaphore->lock);
	}

	--semaphore->counter;
	
	pthread_mutex_unlock(&semaphore->lock);

	return 0;
}
