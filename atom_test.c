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
	int value;
	struct node *next;
}

	TRIVIAL_CLEAR(list, struct node *);

ATOM_TYPE(list, struct node *, list_trivial_clear);

void test_list_swap()
{
	list_atom_t atom = list_atom_init_set(NULL);

	list_atom_swap()
}

int main(int argc, const char *argv[])
{
	test_int_swap();

    return 0;
}
