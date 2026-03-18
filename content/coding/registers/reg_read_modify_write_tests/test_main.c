#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Clear bits [3:0], set to 0x5 */
    uint32_t result = reg_rmw(0xFF, 0x0F, 0x05);
    if (result == 0xF5) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x000000F5, got 0x%08X)\n", result);

    /* TEST 2: Clear bits [15:8], set bits [11:8] to 0xA */
    result = reg_rmw(0xDEADBEEF, 0x0000FF00, 0x00000A00);
    if (result == 0xDEAD0AEF) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0xDEAD0AEF, got 0x%08X)\n", result);

    /* TEST 3: Clear nothing, set bit 31 */
    result = reg_rmw(0x00000001, 0x00000000, 0x80000000);
    if (result == 0x80000001) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x80000001, got 0x%08X)\n", result);

    /* TEST 4: Clear all, set none */
    result = reg_rmw(0xFFFFFFFF, 0xFFFFFFFF, 0x00000000);
    if (result == 0x00000000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x00000000, got 0x%08X)\n", result);

    return 0;
}
