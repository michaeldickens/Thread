/* 
 * 
 * semaphore_test.c
 * ----------------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-22
 * 
 * Test for semaphore.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for usleep
#include "semaphore.h"

semaphore_t trivial_semaphore;

void *trivial_thread_fn(void *args)
{
	for (int i = 0; i < 5; i++) {
		semaphore_wait(&trivial_semaphore);
		printf("Received a signal at i = %d\n", i);
	}

	return NULL;
}

void trivial_test()
{
	semaphore_init(&trivial_semaphore, 2);

	pthread_t thread;
	pthread_create(&thread, NULL, trivial_thread_fn, NULL);

	for (int i = 0; i < 5; i++) {
		printf("sending signal at i = %d\n", i);
		semaphore_signal(&trivial_semaphore);
	}

	pthread_join(thread, NULL);
}

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5

pthread_mutex_t rand_lock;

semaphore_t eating_count;
pthread_mutex_t fork_locks[NUM_FORKS];

void think()
{
	pthread_mutex_lock(&rand_lock);
	/* int value = rand() % 1000; */
	pthread_mutex_unlock(&rand_lock);
	usleep(10000);
}

void eat(size_t id)
{
	printf("Philosopher %d is eating\n", (int) id);
}

void *run_philosopher(void *args)
{
	size_t id = (size_t) args;
	size_t id1 = (id + 1) % NUM_PHILOSOPHERS;
	think();
	semaphore_wait(&eating_count);
	pthread_mutex_lock(&fork_locks[id]);
	pthread_mutex_lock(&fork_locks[id1]);
	eat(id);
	pthread_mutex_unlock(&fork_locks[id1]);
	pthread_mutex_unlock(&fork_locks[id]);
	semaphore_signal(&eating_count);

	return NULL;
}

int main(int argc, const char *argv[])
{
	semaphore_init(&eating_count, 2);
	
	pthread_t threads[NUM_PHILOSOPHERS];
	for (size_t i = 0; i < NUM_PHILOSOPHERS; i++) {
		pthread_create(&threads[i], NULL, run_philosopher, (void *) i);
	}
	for (size_t i = 0; i < NUM_PHILOSOPHERS; i++) {
		pthread_join(threads[i], NULL);
	}

    return 0;
}
