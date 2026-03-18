#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Align 0x1003 to 4-byte boundary -> 0x1004 */
    result = align_up(0x1003, 4);
    if (result == 0x1004) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x1004, got 0x%08X)\n", result);

    /* TEST 2: Already aligned: 0x1000 to 16-byte boundary -> 0x1000 */
    result = align_up(0x1000, 16);
    if (result == 0x1000) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x1000, got 0x%08X)\n", result);

    /* TEST 3: Align 0x0001 to 256-byte boundary -> 0x0100 */
    result = align_up(0x0001, 256);
    if (result == 0x0100) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x0100, got 0x%08X)\n", result);

    /* TEST 4: Align 0x20000007 to 8-byte boundary -> 0x20000008 */
    result = align_up(0x20000007, 8);
    if (result == 0x20000008) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x20000008, got 0x%08X)\n", result);

    return 0;
}
