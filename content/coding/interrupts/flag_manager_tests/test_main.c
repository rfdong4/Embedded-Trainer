#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t flags;
    uint8_t result;

    /* TEST 1: Set flag 5 in empty register -> 0x00000020 */
    flags = set_flag(0x00000000, 5);
    if (flags == 0x00000020) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00000020, got 0x%08X)\n", flags);

    /* TEST 2: Clear flag 3 in 0x000000FF -> 0x000000F7 */
    flags = clear_flag(0x000000FF, 3);
    if (flags == 0x000000F7) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x000000F7, got 0x%08X)\n", flags);

    /* TEST 3: Check flag 7 in 0x00000080 -> 1 */
    result = check_flag(0x00000080, 7);
    if (result == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 1, got %u)\n", result);

    /* TEST 4: Set then clear then check: flag should be 0 */
    flags = set_flag(0x00000000, 10);
    flags = clear_flag(flags, 10);
    result = check_flag(flags, 10);
    if (result == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0, got %u)\n", result);

    return 0;
}
