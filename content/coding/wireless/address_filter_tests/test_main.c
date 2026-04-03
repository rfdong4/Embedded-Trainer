#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Exact match with full mask */
    int r1 = address_match(0x12345678, 0x12345678, 0xFFFFFFFF);
    if (r1 == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %d)\n", r1);

    /* TEST 2: Different address, full mask -> no match */
    int r2 = address_match(0x12345678, 0x12345679, 0xFFFFFFFF);
    if (r2 == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0, got %d)\n", r2);

    /* TEST 3: Different in masked-out bits -> match */
    int r3 = address_match(0x123456FF, 0x12345600, 0xFFFFFF00);
    if (r3 == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 1, got %d)\n", r3);

    /* TEST 4: Zero mask matches everything */
    int r4 = address_match(0xDEADBEEF, 0x00000000, 0x00000000);
    if (r4 == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 1, got %d)\n", r4);

    return 0;
}
