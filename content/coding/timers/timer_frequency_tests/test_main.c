#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: 72MHz / (72 * 1000) = 1000 Hz */
    result = timer_overflow_freq(72000000, 72, 1000);
    if (result == 1000) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1000, got %u)\n", result);

    /* TEST 2: 48MHz / (48 * 500) = 2000 Hz */
    result = timer_overflow_freq(48000000, 48, 500);
    if (result == 2000) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 2000, got %u)\n", result);

    /* TEST 3: 16MHz / (1 * 65535) = 244 Hz (truncated) */
    result = timer_overflow_freq(16000000, 1, 65535);
    if (result == 244) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 244, got %u)\n", result);

    /* TEST 4: 8MHz / (8 * 250) = 4000 Hz */
    result = timer_overflow_freq(8000000, 8, 250);
    if (result == 4000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 4000, got %u)\n", result);

    return 0;
}
