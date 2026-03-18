#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Toggle bits 0,1 in 0x0F -> 0x0C */
    result = toggle_pattern(0x0000000F, 0x00000003);
    if (result == 0x0000000C) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x0000000C, got 0x%08X)\n", result);

    /* TEST 2: Toggle all bits in 0x00000000 -> 0xFFFFFFFF */
    result = toggle_pattern(0x00000000, 0xFFFFFFFF);
    if (result == 0xFFFFFFFF) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0xFFFFFFFF, got 0x%08X)\n", result);

    /* TEST 3: Toggle pattern 0xAA in 0xFF -> 0x55 */
    result = toggle_pattern(0x000000FF, 0x000000AA);
    if (result == 0x00000055) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x00000055, got 0x%08X)\n", result);

    /* TEST 4: Toggle 0x00 pattern (no change) */
    result = toggle_pattern(0x12345678, 0x00000000);
    if (result == 0x12345678) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x12345678, got 0x%08X)\n", result);

    return 0;
}
