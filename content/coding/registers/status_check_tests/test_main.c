#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: N flag set (bit 31) */
    result = check_psr_flag(0x80000000, 0);
    if (result == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %u)\n", result);

    /* TEST 2: Z flag set (bit 30), check Z */
    result = check_psr_flag(0x40000000, 1);
    if (result == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 1, got %u)\n", result);

    /* TEST 3: C flag not set, check C */
    result = check_psr_flag(0x80000000, 2);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: All flags set, check V (bit 28) */
    result = check_psr_flag(0xF0000000, 3);
    if (result == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 1, got %u)\n", result);

    return 0;
}
