#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Bit 0 of 0x01 is set -> 1 */
    result = check_bit(0x01, 0);
    if (result == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %u)\n", result);

    /* TEST 2: Bit 3 of 0x0F is set -> 1 */
    result = check_bit(0x0F, 3);
    if (result == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 1, got %u)\n", result);

    /* TEST 3: Bit 7 of 0x0F is clear -> 0 */
    result = check_bit(0x0F, 7);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: Bit 4 of 0x00 is clear -> 0 */
    result = check_bit(0x00, 4);
    if (result == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0, got %u)\n", result);

    return 0;
}
