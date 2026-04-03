#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    SPSCQueue q;
    uint32_t val;

    /* TEST 1: Init and pop on empty returns 0 */
    spsc_init(&q);
    {
        uint8_t r = spsc_pop(&q, &val);
        if (r == 0) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0 for empty pop, got %u)\n", r);
    }

    /* TEST 2: Push and pop returns correct value */
    {
        spsc_push(&q, 0xABCD);
        uint8_t r = spsc_pop(&q, &val);
        if (r == 1 && val == 0xABCD) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 0xABCD, got 0x%X)\n", val);
    }

    /* TEST 3: Fill queue (7 items for size 8), push returns 0 */
    {
        for (int i = 0; i < 7; i++) spsc_push(&q, i);
        uint8_t r = spsc_push(&q, 99);
        if (r == 0) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0 for full push, got %u)\n", r);
    }

    /* TEST 4: FIFO order preserved */
    {
        spsc_pop(&q, &val);
        uint32_t first = val;
        spsc_pop(&q, &val);
        uint32_t second = val;
        if (first == 0 && second == 1) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 0,1 got %u,%u)\n", first, second);
    }

    return 0;
}
