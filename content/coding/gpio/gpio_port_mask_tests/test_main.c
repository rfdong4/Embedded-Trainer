#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Set lower nibble to 0x05, keep upper nibble 0xF0 */
    result = gpio_masked_write(0xFF, 0x0F, 0x05);
    if (result == 0xF5) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0xF5, got 0x%02X)\n", result);

    /* TEST 2: Mask is 0x00 (no pins affected) -> port unchanged */
    result = gpio_masked_write(0xAB, 0x00, 0xFF);
    if (result == 0xAB) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0xAB, got 0x%02X)\n", result);

    /* TEST 3: Mask is 0xFF (all pins affected) -> values written fully */
    result = gpio_masked_write(0xAB, 0xFF, 0x34);
    if (result == 0x34) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x34, got 0x%02X)\n", result);

    /* TEST 4: Set alternating bits: mask=0xAA, values=0xAA on port 0x00 -> 0xAA */
    result = gpio_masked_write(0x00, 0xAA, 0xAA);
    if (result == 0xAA) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xAA, got 0x%02X)\n", result);

    return 0;
}
