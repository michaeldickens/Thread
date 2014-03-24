/* 
 * 
 * tools.h
 * -------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-24
 * 
 * Tools used by other modules in the Thread library.
 * 
 */

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <pthread.h>

/*
 * Macros to lock or unlock a mutex.
 *
 * x: A pointer to a struct where one of the fields of the struct is a
 * pthread_mutex_t named `lock`.
 */
#define LOCK(x) pthread_mutex_lock(&(x)->lock)
#define UNLOCK(x) pthread_mutex_unlock(&(x)->lock)

/*
 * x: A pointer to a struct where one of the fields of the struct is a
 * pthread_cond_t named `cond`. For WAIT, it must also have a
 * pthread_mutex_t named `lock`.
 */
#define WAIT(x)   pthread_cond_wait(&(x)->cond, &(x)->lock)
#define SIGNAL(x) pthread_cond_signal(&(x)->cond)

#endif /* __TOOLS_H__ */
