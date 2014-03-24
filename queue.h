/* 
 * 
 * queue.h
 * -------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-23
 * 
 * Thread-safe first-in, first-out queue.
 * 
 */

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h> // for malloc
#include "atom.h"

enum {
    ERR_QUEUE_EMPTY = 1
};

#define QUEUE_LOCK(queue)   pthread_mutex_lock(&(queue)->lock)
#define QUEUE_UNLOCK(queue) pthread_mutex_unlock(&(queue)->lock)

/*
 * Queue. Call this to create the functions for managing a queue of a
 * particular type. This queue is NOT thread-safe. A non-thread-safe
 * queue is referred to as a uqueue (short for unsafe queue).
 *
 * Public Functions
 * ----------------
 *
 * int name##_uqueue_enq(name##_uqueue_t *queue, type value)
 * Enqueues `value` to the back of `queue`.
 *
 * int name##_uqueue_deq(name##_uqueue_t *queue, type *res)
 * Dequeues a value from the front of `queue` and copies it to
 * `res`. If the dequeue is successful, returns 0. If it fails
 * (e.g. the queue is empty), returns an error code.
 */
#define QUEUE_TYPE(name, type)                                          \
    typedef struct name##_queue_node_struct {                           \
        type head;                                                      \
        struct name##_queue_node_struct *tail;                          \
    } name##_queue_node_t;                                              \
                                                                        \
    name##_queue_node_t *name##_queue_node_init(type value)             \
    {                                                                   \
        name##_queue_node_t *res = malloc(sizeof(name##_queue_node_t)); \
        if (res == NULL) return NULL;                                   \
        res->head = value;                                              \
        res->tail = NULL;                                               \
        return res;                                                     \
    }                                                                   \
                                                                        \
    typedef struct name##_queue_struct {                                \
        name##_queue_node_t *first;                                     \
        name##_queue_node_t *last;                                      \
        pthread_mutex_t lock;                                           \
    } name##_queue_t;                                                   \
                                                                        \
    name##_queue_t name##_queue_init()                                  \
    {                                                                   \
        name##_queue_t res;                                             \
        res.first = NULL;                                               \
        res.last = NULL;                                                \
        pthread_mutex_init(&res.lock, NULL);                            \
        return res;                                                     \
    }                                                                   \
                                                                        \
    bool name##_queue_empty(name##_queue_t *queue)                      \
    {                                                                   \
        QUEUE_LOCK(queue);                                              \
        bool res = queue->first == NULL;                                \
        QUEUE_UNLOCK(queue);                                            \
        return res;                                                     \
    }                                                                   \
                                                                        \
    int name##_queue_enq(name##_queue_t *queue, type value)             \
    {                                                                   \
        QUEUE_LOCK(queue);                                              \
        if (queue->first == NULL) {                                     \
            queue->first = queue->last = name##_queue_node_init(value); \
        } else {                                                        \
            name##_queue_node_t *saved = queue->last;                   \
                queue->last = name##_queue_node_init(value);            \
                    saved->tail = queue->last;                          \
        }                                                               \
        QUEUE_UNLOCK(queue);                                            \
        return 0;                                                       \
    }                                                                   \
                                                                        \
    int name##_queue_deq(name##_queue_t *queue, type *res)              \
    {                                                                   \
        QUEUE_LOCK(queue);                                              \
        if (queue->first == NULL) {                                     \
            QUEUE_UNLOCK(queue);                                        \
            return ERR_QUEUE_EMPTY;                                     \
        }                                                               \
        name##_queue_node_t *saved = queue->first;                      \
        if (saved == queue->last) {                                     \
            queue->last = NULL;                                         \
        }                                                               \
        queue->first = queue->first->tail;                              \
        QUEUE_UNLOCK(queue);                                            \
                                                                        \
        *res = saved->head;                                             \
        free(saved);                                                    \
        return 0;                                                       \
    }                                                                   

    
