#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: 0x12345678 -> 0x78563412 */
    result = swap_endian(0x12345678);
    if (result == 0x78563412) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x78563412, got 0x%08X)\n", result);

    /* TEST 2: 0x00000001 -> 0x01000000 */
    result = swap_endian(0x00000001);
    if (result == 0x01000000) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x01000000, got 0x%08X)\n", result);

    /* TEST 3: 0xAABBCCDD -> 0xDDCCBBAA */
    result = swap_endian(0xAABBCCDD);
    if (result == 0xDDCCBBAA) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0xDDCCBBAA, got 0x%08X)\n", result);

    /* TEST 4: Double swap returns original: swap(swap(0xDEADBEEF)) == 0xDEADBEEF */
    result = swap_endian(swap_endian(0xDEADBEEF));
    if (result == 0xDEADBEEF) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xDEADBEEF, got 0x%08X)\n", result);

    return 0;
}
