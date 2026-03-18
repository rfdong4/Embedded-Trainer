#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Insert 0x5A into bits 15..8 of 0xFF00FF00 -> 0xFF005A00 */
    result = bit_field_insert(0xFF00FF00, 0x5A, 8, 8);
    if (result == 0xFF005A00) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0xFF005A00, got 0x%08X)\n", result);

    /* TEST 2: Insert 0x7 into bits 2..0 of 0xFFFFFFF0 -> 0xFFFFFFF7 */
    result = bit_field_insert(0xFFFFFFF0, 0x7, 0, 3);
    if (result == 0xFFFFFFF7) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0xFFFFFFF7, got 0x%08X)\n", result);

    /* TEST 3: Insert 0x0 into bits 31..28 of 0xFEDCBA98 -> 0x0EDCBA98 */
    result = bit_field_insert(0xFEDCBA98, 0x0, 28, 4);
    if (result == 0x0EDCBA98) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x0EDCBA98, got 0x%08X)\n", result);

    /* TEST 4: Insert value wider than field - only lowest width bits used.
       Insert 0xFF (8 bits) into a 4-bit field at bits 7..4 of 0x00000000 -> 0x000000F0 */
    result = bit_field_insert(0x00000000, 0xFF, 4, 4);
    if (result == 0x000000F0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x000000F0, got 0x%08X)\n", result);

    return 0;
}
