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
 * Multiple threads may simultaneously access an atom and race
 * conditions are guaranteed not to occur. If two threads attempt to
 * get/set at atom at once, one of the threads blocks until the other
 * finishes.
 */

#include <pthread.h>

/*
 * Creates a trivial clear function that performs no operation.
 */
#define TRIVIAL_CLEAR(name, type)					   \
	void name ## _trivial_clear(type core) {}

#define ATOM_LOCK(atom) pthread_mutex_lock(&(atom)->lock)

#define ATOM_UNLOCK(atom) pthread_mutex_unlock(&(atom)->lock)

/*
 * Atom definition. Call this to allow usage of an atom of a
 * particular type.
 *
 * `name`: The name of the type to use. This name will be used for all
 * functions related to the atom.
 *
 * `type`: The type to use internally with the atom.
 *
 * `clear_fn`: A function with the type signature
 *   int clear_fn(type value)
 *   that clears the value. If the type is not dynamically, allocated,
 *   use the TRIVIAL_CLEAR macro to create a clear function.
 *
 * Public Functions
 * -----------------
 *
 * In the names below, replace `name` with the value passed in for
 * `name`.
 * 
 * name_atom_init: Initializes an atom. Attempting to get or set an
 * atom's value before initializing it results in undefined
 * behavior. This function is NOT thread-safe. That is, you should
 * only initialize an atom once, and only on a single thread.
 *
 * name_atom_init_set: Initializes an atom and sets its initial
 * value. NOT thread-safe. See `name_atom_init` for details.
 * 
 * name_atom_clear: Clears any dynamically-allocated memory associated
 * with an atom. Attempting to clear an atom more than once, clear an
 * atom that was not initialized, or clear an atom for which no value
 * was set results in undefined behavior.
 *
 * name_atom_get: Gets the value from the atom. Blocks until it has
 * unique access to the atom's value. It the atom's value has not been
 * set yet, the return value is undefined.
 *
 * name_atom_set: Sets the value of the atom. Blocks until it has
 * unique access to the atom's value.
 *
 * name_atom_add_condition: ???
 *
 * name_atom_swap: Applies a given function to the contents of the
 * atom. If the value of the atom changes while the function is being
 * applied, calls the function again with the new value. Repeats until
 * it successfully updates the contents of the atom without its
 * contents being changed by any other thread. The given function will
 * be called at least once but may be called more than once, so it
 * should not have any non-repeatable side effects.
 *
 * Note: `name_atom_swap` does not clear the prevously-existing
 * value. It passes it in to the given function, which may clear it if
 * necessary. Remember, however, that the function may be called any
 * number of times.
 *
 * Private Functions
 * -----------------
 *
 * These functions should only be used internally.
 *
 * name_atom_set_unsafe: Sets the value of the atom. Non-blocking and
 * NOT thread-safe.
 */
#define ATOM_TYPE(name, type, clear_fn)							\
	typedef struct name##_atom_struct {							\
		pthread_mutex_t lock;									\
		type core;												\
	} name##_atom_t;											\
																\
	name##_atom_t name##_atom_init()							\
	{															\
		name##_atom_t atom;										\
		pthread_mutex_init(&atom.lock, NULL);			     	\
		return atom;										    \
	}															\
																\
	name##_atom_t name##_atom_init_set(type value)				\
	{															\
		name##_atom_t atom = name##_atom_init();				\
		atom.core = value;										\
		return atom;											\
	}															\
																\
	int name##_atom_clear(name##_atom_t *atom)					\
	{															\
	    ATOM_LOCK(atom);										\
		clear_fn(atom->core);							   		\
        ATOM_UNLOCK(atom);										\
		return 0;												\
	}															\
																\
	type name##_atom_get(name##_atom_t *atom)					\
	{															\
    	ATOM_LOCK(atom);										\
	    type res = atom->core;									\
	    ATOM_UNLOCK(atom);										\
	    return res;												\
	}    														\
																\
	int name##_atom_set_unsafe(name##_atom_t *atom, type value) \
	{															\
		clear_fn(atom->core);									\
		atom->core = value;										\
		return 0;												\
	}															\
																\
	int name##_atom_set(name##_atom_t *atom, type value)		\
	{															\
		ATOM_LOCK(atom);										\
		name##_atom_set_unsafe(atom, value);					\
			ATOM_UNLOCK(atom);									\
		return 0;												\
	}															\
																\
	int name##_atom_swap(name##_atom_t *atom, type (*fn)(type)) \
	{															\
		type value = name##_atom_get(atom);						\
		do {											     	\
			type res = (*fn)(value);							\
			ATOM_LOCK(atom);									\
            if (value == atom->core) {							\
    			atom->core = res;			\
				ATOM_UNLOCK(atom);							    \
				return 0;										\
			}													\
			value = atom->core;									\
			ATOM_UNLOCK(atom);									\
		} while (1);											\
	}															\


/*
 * A macro with equivalent functionality to `name_atom_swap`. Use this
 * if you want to swap using a function that takes more than one
 * argument.
 *
 * name: The name of the type.
 *
 * atom: A pointer to a name_atom_t.
 *
 * fn: The name of a function. Unlike `name_atom_swap`, this function
 * takes a value of the same type as the atom plus any number of
 * additional arguments.
 *
 * The rest of the arguments are values to be passed to `fn`.
 */
#define ATOM_SWAP(name, atom, fn, ...)						\
	do {													\
        type value = name##_atom_get(atom);                 \
		do {										    	\
			type res = fn(value, __VA_ARGS__);				\
			ATOM_LOCK(atom);								\
			if (value == (atom)->core) {					\
                (atom)->core = res;							\
				ATOM_UNLOCK(atom);							\
				return 0;									\
			}												\
			value = (atom)->core;						   	\
            ATOM_UNLOCK(atom);								\
		} while (1);										\
	} while (0);											
	
