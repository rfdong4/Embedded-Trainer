#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: 0x80 (10000000) -> 0x01 (00000001) */
    result = spi_transfer(0x80);
    if (result == 0x01) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x01, got 0x%02X)\n", result);

    /* TEST 2: 0x01 (00000001) -> 0x80 (10000000) */
    result = spi_transfer(0x01);
    if (result == 0x80) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x80, got 0x%02X)\n", result);

    /* TEST 3: 0xF0 (11110000) -> 0x0F (00001111) */
    result = spi_transfer(0xF0);
    if (result == 0x0F) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x0F, got 0x%02X)\n", result);

    /* TEST 4: 0xA5 (10100101) -> 0xA5 (10100101) palindrome */
    result = spi_transfer(0xA5);
    if (result == 0xA5) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xA5, got 0x%02X)\n", result);

    return 0;
}
