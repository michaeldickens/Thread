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

UNSAFE_QUEUE_TYPE(int, int);

void basic_test()
{
    int_uqueue_t q = int_uqueue_init();

    for (size_t i = 0; i < 5; i++) {
        int_uqueue_enq(&q, i + 1);
    }

    int value;
    for (size_t i = 0; i < 5; i++) {
        int_uqueue_deq(&q, &value);
        printf("%d\n", value);
    }

    int code = int_uqueue_deq(&q, &value);
    printf("return code %d, value %d\n", code, value);

}

int main()
{
    basic_test();

    return 0;
}
