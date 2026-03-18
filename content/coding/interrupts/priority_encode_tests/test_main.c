#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    int8_t result;

    /* TEST 1: Only bit 0 set -> 0 */
    result = highest_priority(0x00000001);
    if (result == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0, got %d)\n", result);

    /* TEST 2: Bits 3 and 7 set -> 3 (lowest bit) */
    result = highest_priority(0x00000088);
    if (result == 3) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 3, got %d)\n", result);

    /* TEST 3: No bits set -> -1 */
    result = highest_priority(0x00000000);
    if (result == -1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected -1, got %d)\n", result);

    /* TEST 4: Only bit 31 set -> 31 */
    result = highest_priority(0x80000000);
    if (result == 31) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 31, got %d)\n", result);

    return 0;
}
