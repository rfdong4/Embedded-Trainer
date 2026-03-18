#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Set bit 0 in 0x00 -> 0x01 */
    result = set_bit(0x00, 0);
    if (result == 0x01) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x01, got 0x%02X)\n", result);

    /* TEST 2: Set bit 3 in 0x00 -> 0x08 */
    result = set_bit(0x00, 3);
    if (result == 0x08) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x08, got 0x%02X)\n", result);

    /* TEST 3: Set bit 7 in 0x0F -> 0x8F */
    result = set_bit(0x0F, 7);
    if (result == 0x8F) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x8F, got 0x%02X)\n", result);

    /* TEST 4: Set bit 2 in 0xFF -> 0xFF (already set) */
    result = set_bit(0xFF, 2);
    if (result == 0xFF) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xFF, got 0x%02X)\n", result);

    return 0;
}
