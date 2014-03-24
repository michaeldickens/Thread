/* 
 * 
 * actor_test.c
 * ------------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-24
 * 
 */

#include <stdio.h>
#include <unistd.h> // for sleep
#include "actor.h"

const int NUM_MSGS = 10;

void *actor_fn(actor_t actor)
{
    for (int i = 0; i < NUM_MSGS; i++) {
        int num = (int) actor_receive(&actor);
        printf("received %d\n", num);
    }

    return NULL;
}

void basic_test()
{
    actor_t actor;
    actor_init(&actor, &actor_fn);

    for (int i = 0; i < NUM_MSGS; i++) {
        sleep(1);
        printf("sent %d\n", i + 1);
        actor_send(&actor, (void *) (i + 1));
    }

    actor_join(&actor);
}

int main()
{
    basic_test();

    return 0;
}
