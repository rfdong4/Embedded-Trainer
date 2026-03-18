#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Single task expires after 2 ticks */
    delay_queue_init();
    delay_task(0, 2);
    uint8_t expired = delay_tick_update();  /* tick 1: delay_ticks[0] = 1, not expired yet */
    if (expired == 0x00) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00, got 0x%02X)\n", expired);

    /* TEST 2: Task 0 expires on second tick */
    expired = delay_tick_update();  /* tick 2: delay_ticks[0] = 0, expired */
    if (expired == 0x01) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x01, got 0x%02X)\n", expired);

    /* TEST 3: Multiple tasks expire at different times */
    delay_queue_init();
    delay_task(1, 1);  /* expires after 1 tick */
    delay_task(3, 2);  /* expires after 2 ticks */
    expired = delay_tick_update();  /* tick 1: task 1 expires */
    if (expired == 0x02) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x02, got 0x%02X)\n", expired);

    /* TEST 4: Task 3 expires on next tick, task 1 already gone */
    expired = delay_tick_update();  /* tick 2: task 3 expires */
    if (expired == 0x08) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x08, got 0x%02X)\n", expired);

    return 0;
}
