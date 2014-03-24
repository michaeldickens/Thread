/* 
 * 
 * actor.c
 * -------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-24
 * 
 */

#include "actor.h"

QUEUE_DEFINE(actor, void *);

void *actor_thread_fn(void *args)
{
    struct actor_thread_data *data = (struct actor_thread_data *) args;
    void *res = (*data->fn)(data->actor);
    free(data);
    return res;
}

int actor_init(actor_t *actor, void *(*fn)(actor_t))
{
    actor->core = malloc(sizeof(struct actor_core));
    if (actor->core == NULL) return ERR_MEMORY;
    actor->core->queue = actor_queue_init();
    actor->actor_thread = false;

    struct actor_thread_data *data = malloc(sizeof(struct actor_thread_data));
    if (data == NULL) {
        free(actor->core);
        return ERR_MEMORY;
    }
    data->actor = *actor;
    data->actor.actor_thread = true;
    data->fn = fn;

    pthread_create(&actor->core->thread, NULL, actor_thread_fn,
                   (void *) data);
    pthread_mutex_init(&actor->core->lock, NULL);
    pthread_cond_init(&actor->core->cond, NULL);

    return 0;
}

void *actor_join(actor_t *actor)
{
    if (actor->actor_thread) {
        /* Cannot join from the actor thread. */
        return NULL;
    }

    void *res = NULL;
    pthread_join(actor->core->thread, &res);
    return res;
}

int actor_send(actor_t *actor, void *data)
{
    actor_queue_enq(&actor->core->queue, data);
    SIGNAL(actor->core);

    return 0;
}

void *actor_receive(actor_t *actor)
{
    if (!actor->actor_thread) {
        /* Cannot call `receive` unless on the actor thread. */
        return NULL;
    }
        
    void *data = NULL;
    
    LOCK(actor->core);
    while (actor_queue_empty(&actor->core->queue)) {
        WAIT(actor->core);        
    }
    actor_queue_deq(&actor->core->queue, &data);

    UNLOCK(actor->core);
    return data;
}
