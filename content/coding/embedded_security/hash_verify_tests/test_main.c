#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Matching 4-byte hashes */
    uint8_t comp1[] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t exp1[]  = {0xDE, 0xAD, 0xBE, 0xEF};
    int r1 = hash_verify(comp1, exp1, 4);
    if (r1 == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %d)\n", r1);

    /* TEST 2: Mismatched hashes (last byte differs) */
    uint8_t comp2[] = {0xDE, 0xAD, 0xBE, 0xEF};
    uint8_t exp2[]  = {0xDE, 0xAD, 0xBE, 0xFF};
    int r2 = hash_verify(comp2, exp2, 4);
    if (r2 == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0, got %d)\n", r2);

    /* TEST 3: Single byte match */
    uint8_t comp3[] = {0x42};
    uint8_t exp3[]  = {0x42};
    int r3 = hash_verify(comp3, exp3, 1);
    if (r3 == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 1, got %d)\n", r3);

    /* TEST 4: Completely different 8-byte hashes */
    uint8_t comp4[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    uint8_t exp4[]  = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88};
    int r4 = hash_verify(comp4, exp4, 8);
    if (r4 == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0, got %d)\n", r4);

    return 0;
}
