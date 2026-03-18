#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "solution.c"

int main(void) {
    uint8_t queue[8];
    uint8_t size;

    /* TEST 1: Insert into empty queue */
    memset(queue, 0, sizeof(queue));
    size = 0;
    pq_insert(queue, &size, 5, 8);
    if (size == 1 && queue[0] == 5) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected size=1 queue[0]=5, got size=%u queue[0]=%u)\n", size, queue[0]);

    /* TEST 2: Insert maintaining sorted order */
    memset(queue, 0, sizeof(queue));
    size = 0;
    pq_insert(queue, &size, 5, 8);
    pq_insert(queue, &size, 3, 8);
    pq_insert(queue, &size, 7, 8);
    if (size == 3 && queue[0] == 3 && queue[1] == 5 && queue[2] == 7) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected [3,5,7] size=3, got [%u,%u,%u] size=%u)\n",
                queue[0], queue[1], queue[2], size);

    /* TEST 3: Insert at beginning (highest priority) */
    memset(queue, 0, sizeof(queue));
    size = 0;
    pq_insert(queue, &size, 10, 8);
    pq_insert(queue, &size, 20, 8);
    pq_insert(queue, &size, 1, 8);
    if (size == 3 && queue[0] == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected queue[0]=1 size=3, got queue[0]=%u size=%u)\n", queue[0], size);

    /* TEST 4: Full queue rejects insert */
    memset(queue, 0, sizeof(queue));
    size = 0;
    {
        uint8_t i;
        for (i = 0; i < 8; i++) pq_insert(queue, &size, i * 10, 8);
    }
    pq_insert(queue, &size, 5, 8);
    if (size == 8) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected size=8, got size=%u)\n", size);

    return 0;
}
