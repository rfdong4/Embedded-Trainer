#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Reverse 0xB1 (10110001) -> 0x8D (10001101) */
    result = bit_reverse(0xB1);
    if (result == 0x8D) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x8D, got 0x%02X)\n", result);

    /* TEST 2: Reverse 0x01 (00000001) -> 0x80 (10000000) */
    result = bit_reverse(0x01);
    if (result == 0x80) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x80, got 0x%02X)\n", result);

    /* TEST 3: Reverse 0xFF (11111111) -> 0xFF (11111111) */
    result = bit_reverse(0xFF);
    if (result == 0xFF) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0xFF, got 0x%02X)\n", result);

    /* TEST 4: Reverse 0x6C (01101100) -> 0x36 (00110110) */
    result = bit_reverse(0x6C);
    if (result == 0x36) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x36, got 0x%02X)\n", result);

    return 0;
}
