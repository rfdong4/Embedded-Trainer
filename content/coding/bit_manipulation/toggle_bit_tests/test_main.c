#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Toggle bit 0 in 0x00 -> 0x01 */
    result = toggle_bit(0x00, 0);
    if (result == 0x01) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x01, got 0x%02X)\n", result);

    /* TEST 2: Toggle bit 3 in 0x0F -> 0x07 */
    result = toggle_bit(0x0F, 3);
    if (result == 0x07) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x07, got 0x%02X)\n", result);

    /* TEST 3: Toggle bit 7 in 0x00 -> 0x80 */
    result = toggle_bit(0x00, 7);
    if (result == 0x80) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x80, got 0x%02X)\n", result);

    /* TEST 4: Toggle bit 4 in 0xFF -> 0xEF */
    result = toggle_bit(0xFF, 4);
    if (result == 0xEF) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xEF, got 0x%02X)\n", result);

    return 0;
}
