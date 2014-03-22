/* 
 * 
 * atom.h
 * ------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-21
 * 
 * A type for performing thread-safe atomic operations on a value.
 * 
 */


// TODO: Try doing this the way the BSD libraries handle templating,
// where you use macros to emulate C++ templates.

/*
 * Internals:
 * The first few bytes of the atom contain the atom_data struct.
 * The rest contain the value.
 */
typedef void * atom_t;

/*
 * Initializes an atom of the given size.
 *
 * size: The size of the atom.
 *
 * clear: A function that clears the contents of the atom, or NULL if
 * the contents of the atom are not dynamically allocated.
 */
atom_t atom_init(size_t size, void (*clear)(void *));

/*
 * Clears any memory associated with an atom.
 */
int atom_clear(atom_t atom);

/*
 * Gets the value of the atom. If the atom's value has not been set,
 * returns NULL. Blocks until the atom can be read.
 */
const void *atom_get(atom_t atom);

int atom_set(atom_t atom, void *value);

int atom_swap(atom_t atom, void *(*setter)(const void *));
