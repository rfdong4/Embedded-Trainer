#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: 0x48 (01001000) vs 0x50 (01010000) -> differ at bit 4, master1 has 0 -> master1 wins */
    result = i2c_arbitrate(0x48, 0x50);
    if (result == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %u)\n", result);

    /* TEST 2: 0x68 (01101000) vs 0x48 (01001000) -> differ at bit 5, master2 has 0 -> master2 wins */
    result = i2c_arbitrate(0x68, 0x48);
    if (result == 2) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 2, got %u)\n", result);

    /* TEST 3: Both masters send same address 0x3C -> tie (0) */
    result = i2c_arbitrate(0x3C, 0x3C);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: 0x00 (00000000) vs 0xFF (11111111) -> differ at bit 7, master1 has 0 -> master1 wins */
    result = i2c_arbitrate(0x00, 0xFF);
    if (result == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 1, got %u)\n", result);

    return 0;
}
