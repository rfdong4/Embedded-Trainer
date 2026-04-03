#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Enable peripheral 0 in empty register */
    result = clock_enable(0x00000000, 0);
    if (result == 0x00000001) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00000001, got 0x%08X)\n", result);

    /* TEST 2: Enable peripheral 7, then disable peripheral 3 */
    result = clock_enable(0x00000000, 7);
    result = clock_enable(result, 3);
    result = clock_disable(result, 3);
    if (result == 0x00000080) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x00000080, got 0x%08X)\n", result);

    /* TEST 3: Disable peripheral 4 from 0xFF */
    result = clock_disable(0x000000FF, 4);
    if (result == 0x000000EF) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x000000EF, got 0x%08X)\n", result);

    /* TEST 4: Enable peripheral 31 */
    result = clock_enable(0x00000000, 31);
    if (result == 0x80000000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x80000000, got 0x%08X)\n", result);

    return 0;
}
