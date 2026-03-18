#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: 0x0F (00001111) has 4 set bits */
    result = count_set_bits(0x0000000F);
    if (result == 4) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 4, got %u)\n", result);

    /* TEST 2: 0xFFFFFFFF has 32 set bits */
    result = count_set_bits(0xFFFFFFFF);
    if (result == 32) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 32, got %u)\n", result);

    /* TEST 3: 0x00000000 has 0 set bits */
    result = count_set_bits(0x00000000);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: 0xA5A5A5A5 (10100101 repeated) has 16 set bits */
    result = count_set_bits(0xA5A5A5A5);
    if (result == 16) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 16, got %u)\n", result);

    return 0;
}
