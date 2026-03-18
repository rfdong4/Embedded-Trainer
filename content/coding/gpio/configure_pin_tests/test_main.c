#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Set pin 0 as output in empty register */
    result = configure_pin(0x00000000, 0, 1);
    if (result == 0x00000001) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00000001, got 0x%08X)\n", result);

    /* TEST 2: Set pin 15 as output */
    result = configure_pin(0x00000000, 15, 1);
    if (result == 0x00008000) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x00008000, got 0x%08X)\n", result);

    /* TEST 3: Set pin 3 as input (clear bit) in 0x000000FF */
    result = configure_pin(0x000000FF, 3, 0);
    if (result == 0x000000F7) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x000000F7, got 0x%08X)\n", result);

    /* TEST 4: Set pin 31 as output */
    result = configure_pin(0x00000000, 31, 1);
    if (result == 0x80000000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x80000000, got 0x%08X)\n", result);

    return 0;
}
