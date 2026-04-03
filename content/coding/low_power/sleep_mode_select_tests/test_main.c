#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t mode;

    /* TEST 1: Peripherals active, tight latency => RUN (0) */
    mode = select_sleep_mode(1, 1, 5);
    if (mode == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0, got %u)\n", mode);

    /* TEST 2: Peripherals active, relaxed latency => SLEEP (1) */
    mode = select_sleep_mode(1, 0, 100);
    if (mode == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 1, got %u)\n", mode);

    /* TEST 3: No peripherals, need SRAM => STOP (2) */
    mode = select_sleep_mode(0, 1, 1000);
    if (mode == 2) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 2, got %u)\n", mode);

    /* TEST 4: No peripherals, no SRAM needed => STANDBY (3) */
    mode = select_sleep_mode(0, 0, 10000);
    if (mode == 3) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 3, got %u)\n", mode);

    return 0;
}
