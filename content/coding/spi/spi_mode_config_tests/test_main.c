#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Mode 0 -> CPOL=0, CPHA=0 -> 0x00 */
    result = spi_mode_to_config(0);
    if (result == 0x00) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00, got 0x%02X)\n", result);

    /* TEST 2: Mode 3 -> CPOL=1, CPHA=1 -> 0x03 */
    result = spi_mode_to_config(3);
    if (result == 0x03) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x03, got 0x%02X)\n", result);

    /* TEST 3: Mode 2 -> CPOL=1, CPHA=0 -> 0x02 */
    result = spi_mode_to_config(2);
    if (result == 0x02) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x02, got 0x%02X)\n", result);

    /* TEST 4: Invalid mode 5 -> 0xFF */
    result = spi_mode_to_config(5);
    if (result == 0xFF) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xFF, got 0x%02X)\n", result);

    return 0;
}
