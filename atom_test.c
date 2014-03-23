/* 
 * 
 * atom_test.c
 * -----------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-22
 * 
 * Tests for the Atom type.
 * 
 */

#include <stdio.h>
#include <stdlib.h> // for malloc
#include <unistd.h> // for sleep
#include "atom.h"

TRIVIAL_CLEAR(int, int);

ATOM_TYPE(int, int, int_trivial_clear);

int take_a_long_time(int x)
{
	sleep(1);
	return x * x;
}

void *test_int_swap_thread2(void *args)
{
	int_atom_t *atom = (int_atom_t *) args;
	int_atom_set(atom, 6);
	sleep(1);
	int_atom_set(atom, 7);

	return NULL;
}

void test_int_swap()
{
    int_atom_t atom = int_atom_init_set(5);

	pthread_t thread;
	pthread_create(&thread, NULL, test_int_swap_thread2, (void *) &atom);

	int_atom_swap(&atom, take_a_long_time);
	printf("swapped atom to %d\n", int_atom_get(&atom));

	int_atom_clear(&atom);
	pthread_join(thread, NULL);
}

struct node {
	int head;
	struct node *tail;
};

TRIVIAL_CLEAR(list, struct node *);

ATOM_TYPE(list, struct node *, list_trivial_clear);

struct node *cons(struct node *xs, int x)
{
	usleep(10000);
	struct node *res = malloc(sizeof(struct node));
	res->head = x;
	res->tail = xs;
	return res;
}

void print_list(struct node *xs)
{
	if (xs == NULL) {
		printf("nil\n");
	} else {
		printf("%d : ", xs->head);
		print_list(xs->tail);
	}
}

int length(struct node *xs)
{
	if (xs == NULL) return 0;
	else return 1 + length(xs->tail);
}

void *test_list_swap_2(void *args)
{
	list_atom_t *atom_ptr = (list_atom_t *) args;
	for (size_t i = 0; i < 20; i++) {
		ATOM_SWAP(list, atom_ptr, cons, 2);
	}

	return NULL;
}

void test_list_swap()
{
	list_atom_t atom = list_atom_init_set(NULL);

	pthread_t thread;
	pthread_create(&thread, NULL, test_list_swap_2, (void *) &atom);

	for (size_t i = 0; i < 20; i++) {
		ATOM_SWAP(list, &atom, cons, 1);
	}

	pthread_join(thread, NULL);

	print_list(list_atom_get(&atom));
	printf("length: %d\n", length(list_atom_get(&atom)));
}

int main()
{
	test_list_swap();

    return 0;
}
