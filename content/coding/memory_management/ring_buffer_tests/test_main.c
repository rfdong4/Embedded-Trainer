#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    RingBuffer rb;

    /* TEST 1: Push and pop a single value */
    rb_init(&rb);
    rb_push(&rb, 0x42);
    {
        int16_t val = rb_pop(&rb);
        if (val == 0x42) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0x42, got 0x%04X)\n", (uint16_t)val);
    }

    /* TEST 2: Pop from empty buffer returns -1 */
    rb_init(&rb);
    {
        int16_t val = rb_pop(&rb);
        if (val == -1) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected -1, got %d)\n", val);
    }

    /* TEST 3: Fill buffer completely, push to full returns 0 */
    rb_init(&rb);
    {
        uint8_t i;
        uint8_t all_ok = 1;
        for (i = 0; i < RB_SIZE; i++) {
            if (rb_push(&rb, i) != 1) { all_ok = 0; break; }
        }
        if (all_ok && rb_push(&rb, 0xFF) == 0) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (buffer full detection failed)\n");
    }

    /* TEST 4: FIFO order preserved */
    rb_init(&rb);
    rb_push(&rb, 0xAA);
    rb_push(&rb, 0xBB);
    rb_push(&rb, 0xCC);
    {
        int16_t a = rb_pop(&rb);
        int16_t b = rb_pop(&rb);
        int16_t c = rb_pop(&rb);
        if (a == 0xAA && b == 0xBB && c == 0xCC) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected AA,BB,CC got %02X,%02X,%02X)\n",
                     (uint8_t)a, (uint8_t)b, (uint8_t)c);
    }

    return 0;
}
