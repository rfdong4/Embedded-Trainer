#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Extract upper nibble of 0xAB -> 0x0A */
    result = extract_field(0xAB, 4, 4);
    if (result == 0x0A) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x0A, got 0x%02X)\n", result);

    /* TEST 2: Extract lower nibble of 0xAB -> 0x0B */
    result = extract_field(0xAB, 0, 4);
    if (result == 0x0B) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x0B, got 0x%02X)\n", result);

    /* TEST 3: Extract bits 2..4 (3 bits) of 0b11010110 (0xD6) -> 0b101 = 5 */
    result = extract_field(0xD6, 2, 3);
    if (result == 0x05) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x05, got 0x%02X)\n", result);

    /* TEST 4: Extract single bit 7 of 0x80 -> 1 */
    result = extract_field(0x80, 7, 1);
    if (result == 0x01) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x01, got 0x%02X)\n", result);

    return 0;
}
