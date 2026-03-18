#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Address 0x50, write -> 0xA0 */
    result = i2c_addr_byte(0x50, 0);
    if (result == 0xA0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0xA0, got 0x%02X)\n", result);

    /* TEST 2: Address 0x50, read -> 0xA1 */
    result = i2c_addr_byte(0x50, 1);
    if (result == 0xA1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0xA1, got 0x%02X)\n", result);

    /* TEST 3: Address 0x27, write -> 0x4E */
    result = i2c_addr_byte(0x27, 0);
    if (result == 0x4E) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x4E, got 0x%02X)\n", result);

    /* TEST 4: Address 0x00, read -> 0x01 (general call read) */
    result = i2c_addr_byte(0x00, 1);
    if (result == 0x01) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x01, got 0x%02X)\n", result);

    return 0;
}
