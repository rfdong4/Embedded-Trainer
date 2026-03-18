#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Write and read R0 */
    reg_write(0, 0xDEADBEEF);
    result = reg_read(0);
    if (result == 0xDEADBEEF) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0xDEADBEEF, got 0x%08X)\n", result);

    /* TEST 2: Write and read R7 */
    reg_write(7, 0x12345678);
    result = reg_read(7);
    if (result == 0x12345678) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x12345678, got 0x%08X)\n", result);

    /* TEST 3: Out-of-range read returns 0 */
    result = reg_read(8);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x00000000, got 0x%08X)\n", result);

    /* TEST 4: R0 unchanged after out-of-range write */
    reg_write(10, 0xFFFFFFFF);
    result = reg_read(0);
    if (result == 0xDEADBEEF) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xDEADBEEF, got 0x%08X)\n", result);

    return 0;
}
