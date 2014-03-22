/* 
 * 
 * atom.c
 * ------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-21
 * 
 */

#include <pthread.h>
#include <stdbool.h>
#include <string.h>

struct atom_data {
	size_t size;
	void (*clear)(void *);
	bool initialized;
	pthread_mutex_t lock;
};

atom_t atom_init(size_t size, void (*clear)(void *))
{
	struct atom_data data;
	data.size = size;
	data.clear = clear;
	data.initialized = false;
	pthread_mutex_init(&data.lock);
	atom_t atom = malloc(sizeof(data) + size);
	memcpy(atom, &data, sizeof(data));
	
	return atom;
}

const void *atom_get(atom_t atom)
{
	pthread_mutex_lock(&data->lock);

	struct atom_data *data = (struct atom_data *) atom;
	if (!data->initalized) {
		pthread_mutex_unlock(&data->lock);
		return NULL;
	}

	void *res = (void *) ((char *) atom + sizeof(data));

	pthread_mutex_unlock(&data->lock);
	return res;
}
