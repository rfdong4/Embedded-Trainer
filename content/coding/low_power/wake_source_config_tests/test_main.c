#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Only WKUP1 enabled => 0x01 */
    result = configure_wake_sources(1, 0, 0, 0, 0, 0, 0, 0);
    if (result == 0x01) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x01, got 0x%08X)\n", result);

    /* TEST 2: RTC alarm + UART detect => bits 2 and 4 => 0x14 */
    result = configure_wake_sources(0, 0, 1, 0, 1, 0, 0, 0);
    if (result == 0x14) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x14, got 0x%08X)\n", result);

    /* TEST 3: All sources enabled => 0xFF */
    result = configure_wake_sources(1, 1, 1, 1, 1, 1, 1, 1);
    if (result == 0xFF) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0xFF, got 0x%08X)\n", result);

    /* TEST 4: No sources => 0x00 */
    result = configure_wake_sources(0, 0, 0, 0, 0, 0, 0, 0);
    if (result == 0x00) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x00, got 0x%08X)\n", result);

    return 0;
}
