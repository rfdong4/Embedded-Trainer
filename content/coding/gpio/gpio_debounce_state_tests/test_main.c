#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: All 1s in last 4 samples -> stable HIGH (1) */
    result = gpio_debounce(0xFF, 4);
    if (result == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %u)\n", result);

    /* TEST 2: Last 4 bits are 0000 (history=0xF0) -> stable LOW (0) */
    result = gpio_debounce(0xF0, 4);
    if (result == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0, got %u)\n", result);

    /* TEST 3: Last 4 bits are 0101 (history=0xF5) -> unstable (2) */
    result = gpio_debounce(0xF5, 4);
    if (result == 2) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 2, got %u)\n", result);

    /* TEST 4: All 8 samples are 0 (history=0x00) -> stable LOW (0) */
    result = gpio_debounce(0x00, 8);
    if (result == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0, got %u)\n", result);

    return 0;
}
