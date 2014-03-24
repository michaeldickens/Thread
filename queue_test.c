/* 
 * 
 * queue_test.c
 * ------------
 * 
 * Author: Michael Dickens <mdickens93@gmail.com>
 * Created: 2014-03-24
 * 
 */

#include <stdio.h>
#include "queue.h"

QUEUE_TYPE(int, int);

const int NUM_ELEMS = 10000;

void *basic_test_thread2(void *args)
{
    int_queue_t *q = (int_queue_t *) args;

    for (int i = 0; i < NUM_ELEMS; i++) {
        printf("equeuing %d\n", i + 1);
        int_queue_enq(q, i + 1);
    }

    return NULL;
}

void basic_test()
{
    int_queue_t q = int_queue_init();

    pthread_t thread;
    pthread_create(&thread, NULL, basic_test_thread2, (void *) &q);

    int value;
    int sum = 0;
    for (size_t i = 0; i < NUM_ELEMS; ) {
        int code = int_queue_deq(&q, &value);
        if (code == 0) {
            sum += value;
            printf("%d\n", value);
            i++;
        } else {
            printf("code %d\n", code);
        }
    }

    pthread_join(thread, NULL);

    printf("sum = %d (expected %d)\n", sum, NUM_ELEMS * (NUM_ELEMS + 1) / 2);
}

int main()
{
    basic_test();

    return 0;
}
