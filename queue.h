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

#include <stdbool.h>
#include <stdlib.h> // for malloc
#include "atom.h"

enum {
    ERR_QUEUE_EMPTY = 1
};

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
#define UNSAFE_QUEUE_TYPE(name, type)                                   \
    typedef struct name##_uqueue_node_struct {                          \
        type head;                                                      \
        struct name##_uqueue_node_struct *tail;                         \
    } name##_uqueue_node_t;                                             \
                                                                        \
    name##_uqueue_node_t *name##_uqueue_node_init(type value)           \
    {                                                                   \
        name##_uqueue_node_t *res = malloc(sizeof(name##_uqueue_node_t)); \
        if (res == NULL) return NULL;                                   \
        res->head = value;                                              \
        res->tail = NULL;                                               \
        return res;                                                     \
    }                                                                   \
                                                                        \
    typedef struct name##_uqueue_struct {                               \
        name##_uqueue_node_t *first;                                    \
        name##_uqueue_node_t *last;                                     \
    } name##_uqueue_t;                                                  \
                                                                        \
    name##_uqueue_t name##_uqueue_init()                                \
    {                                                                   \
        name##_uqueue_t res;                                            \
        res.first = NULL;                                               \
        res.last = NULL;                                                \
        return res;                                                     \
    }                                                                   \
                                                                        \
    bool name##_uqueue_empty(name##_uqueue_t *queue)                    \
    {                                                                   \
        return queue->first == NULL;                                    \
    }                                                                   \
                                                                        \
    int name##_uqueue_enq(name##_uqueue_t *queue, type value)           \
    {                                                                   \
        if (queue->last == NULL) {                                      \
            queue->first = queue->last = name##_uqueue_node_init(value); \
        } else {                                                        \
            name##_uqueue_node_t *saved = queue->last;                  \
            queue->last = name##_uqueue_node_init(value);               \
            saved->tail = queue->last;                                  \
        }                                                               \
        return 0;                                                       \
    }                                                                   \
                                                                        \
    int name##_uqueue_deq(name##_uqueue_t *queue, type *res)            \
    {                                                                   \
        if (queue->first == NULL) return ERR_QUEUE_EMPTY;               \
        name##_uqueue_node_t *saved = queue->first;                     \
        queue->first = queue->first->tail;                              \
        *res = saved->head;                                             \
        free(saved);                                                    \
        return 0;                                                       \
    }                                                                   

/*
 * A thread-safe queue that builds on top of uqueue (non-thread-safe
 * queue).
*/
#define QUEUE_TYPE(name, type)                                          \
    UNSAFE_QUEUE_TYPE(name, type)                                       \
    TRIVIAL_CLEAR(name##_uqueue, name##_uqueue_t)                       \
    ATOM_TYPE(name##_queue, name##_uqueue_t, name##_uqueue_trivial_clear) \
                                                                        \
    typedef name##_queue_atom_t name##_queue_t;                         \
                                                                        \
    name##_queue_t name##_queue_init()                                  \
    {                                                                   \
        name##_uqueue_t empty = name##_uqueue_init();                   \
        return name##_queue_atom_init_set(empty);                       \
    }                                                                   \
                                                                        \
    int name##_queue_enq(name##_queue_t *queue, type value)             \
    {                                                                   \
        return 0;                                                       \
    }                                                                   \
    // Dequeue is the tricky part. How to we extract a value just
    // using swap()?
    
    
    
