#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Identical arrays */
    uint8_t a1[] = {0x01, 0x02, 0x03, 0x04};
    uint8_t b1[] = {0x01, 0x02, 0x03, 0x04};
    int r1 = secure_compare(a1, b1, 4);
    if (r1 == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %d)\n", r1);

    /* TEST 2: Differ in first byte */
    uint8_t a2[] = {0xFF, 0x02, 0x03, 0x04};
    uint8_t b2[] = {0x00, 0x02, 0x03, 0x04};
    int r2 = secure_compare(a2, b2, 4);
    if (r2 == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0, got %d)\n", r2);

    /* TEST 3: Differ in last byte */
    uint8_t a3[] = {0xAA, 0xBB, 0xCC, 0xDD};
    uint8_t b3[] = {0xAA, 0xBB, 0xCC, 0xDE};
    int r3 = secure_compare(a3, b3, 4);
    if (r3 == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %d)\n", r3);

    /* TEST 4: Single byte match */
    uint8_t a4[] = {0x55};
    uint8_t b4[] = {0x55};
    int r4 = secure_compare(a4, b4, 1);
    if (r4 == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 1, got %d)\n", r4);

    return 0;
}
