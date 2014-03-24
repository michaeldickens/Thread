/* 
 * 
 * actor.h
 * -------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-24
 * 
 * An actor is an abstraction over a thread. Other threads may send
 * messages to an actor, and the actor may receive messages.
 *
 * If a thread sends a message to an actor, the message waits in the
 * message queue until the actor calls `receive`.
 * 
 */

#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <stdbool.h>
#include "tools.h"
#include "queue.h"

QUEUE_DECLARE(actor, void *);

enum {
    ERR_MEMORY = 1,
};

struct actor_core {
    actor_queue_t queue;
    pthread_t thread;
    pthread_mutex_t lock;
    pthread_cond_t cond;    
};

typedef struct actor_struct {
    struct actor_core *core;

    /* true if this actor is currently on the thread for the actor, false
     * otherwise. Used to ensure that only the actor thread can receive
     * messages.
     */
    bool actor_thread;
} actor_t;

struct actor_thread_data {
    actor_t actor;
    void *(*fn)(actor_t);
};

/*
 * Initializes an actor.
 *
 * fn: A function on which the actor thread runs. `fn` should take an
 * `actor_t` as its only argument.
 */
int actor_init(actor_t *actor, void *(*fn)(actor_t));

/*
 * Joins the actor thread to the current thread. Should not be called
 * from the actor thread. Returns the result of the actor function
 * passed to `actor_init`.
 */
void *actor_join(actor_t *actor);

/*
 * Sends a message to `actor` containing `data`. Non-interrupting: the
 * actor will not receive the message until it requests it using `receive`.
 */
int actor_send(actor_t *actor, void *data);

/*
 * Receives a message. If the message queue is empty, blocks until a
 * message is enqueued.
 */
void *actor_receive(actor_t *actor);

#endif /* __ACTOR_H__ */
