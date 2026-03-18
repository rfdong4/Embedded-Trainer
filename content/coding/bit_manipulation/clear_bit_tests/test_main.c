#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Clear bit 0 in 0xFF -> 0xFE */
    result = clear_bit(0xFF, 0);
    if (result == 0xFE) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0xFE, got 0x%02X)\n", result);

    /* TEST 2: Clear bit 3 in 0xFF -> 0xF7 */
    result = clear_bit(0xFF, 3);
    if (result == 0xF7) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0xF7, got 0x%02X)\n", result);

    /* TEST 3: Clear bit 7 in 0x8F -> 0x0F */
    result = clear_bit(0x8F, 7);
    if (result == 0x0F) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x0F, got 0x%02X)\n", result);

    /* TEST 4: Clear bit 4 in 0x00 -> 0x00 (already clear) */
    result = clear_bit(0x00, 4);
    if (result == 0x00) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x00, got 0x%02X)\n", result);

    return 0;
}
