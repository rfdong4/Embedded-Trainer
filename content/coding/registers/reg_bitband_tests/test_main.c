#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Base address, bit 0 => 0x42000000 */
    uint32_t result = calc_bitband_addr(0x40000000, 0);
    if (result == 0x42000000) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x42000000, got 0x%08X)\n", result);

    /* TEST 2: Address 0x40000000, bit 7 => 0x42000000 + 0 + 28 = 0x4200001C */
    result = calc_bitband_addr(0x40000000, 7);
    if (result == 0x4200001C) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x4200001C, got 0x%08X)\n", result);

    /* TEST 3: Address 0x40000004, bit 0 => 0x42000000 + 128 + 0 = 0x42000080 */
    result = calc_bitband_addr(0x40000004, 0);
    if (result == 0x42000080) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x42000080, got 0x%08X)\n", result);

    /* TEST 4: Address 0x40000010, bit 3 => 0x42000000 + (16*32) + (3*4) = 0x42000000 + 512 + 12 = 0x4200020C */
    result = calc_bitband_addr(0x40000010, 3);
    if (result == 0x4200020C) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x4200020C, got 0x%08X)\n", result);

    return 0;
}
