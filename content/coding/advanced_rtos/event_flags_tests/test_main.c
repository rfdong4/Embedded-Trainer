#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t flags = 0;

    /* TEST 1: Set flags and check with wait ANY */
    event_set(&flags, 0x05);  /* Set bits 0 and 2 */
    if (event_wait(flags, 0x01, 0) == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1 for wait ANY)\n");

    /* TEST 2: Wait ALL — not all requested bits set */
    if (event_wait(flags, 0x07, 1) == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0 for wait ALL with missing bit 1)\n");

    /* TEST 3: Set more flags, now wait ALL succeeds */
    event_set(&flags, 0x02);  /* Now flags = 0x07 */
    if (event_wait(flags, 0x07, 1) == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 1 for wait ALL)\n");

    /* TEST 4: Clear flags, wait ANY fails */
    event_clear(&flags, 0x07);
    if (event_wait(flags, 0x07, 0) == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0 after clearing all flags)\n");

    return 0;
}
