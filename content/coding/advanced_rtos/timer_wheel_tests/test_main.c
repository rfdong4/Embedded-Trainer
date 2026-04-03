#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    TimerWheel tw;

    /* TEST 1: Init and tick on empty slot returns -1 */
    timer_wheel_init(&tw);
    {
        int8_t r = timer_tick(&tw, 0);
        if (r == -1) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected -1, got %d)\n", r);
    }

    /* TEST 2: Add timer at tick 5, tick 5 returns it */
    {
        timer_add(&tw, 5, 42);
        int8_t r = timer_tick(&tw, 5);
        if (r == 42) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 42, got %d)\n", r);
    }

    /* TEST 3: After firing, slot is empty */
    {
        int8_t r = timer_tick(&tw, 5);
        if (r == -1) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected -1 after consumed, got %d)\n", r);
    }

    /* TEST 4: Add to occupied slot fails */
    {
        timer_add(&tw, 3, 10);
        uint8_t ok = timer_add(&tw, 11, 20);  /* 11 % 8 == 3, same slot */
        if (ok == 0) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 0 for occupied slot, got %u)\n", ok);
    }

    return 0;
}
