#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Normal case, timeout not elapsed: start=100, now=150, timeout=100 -> 0 */
    result = timeout_elapsed(100, 150, 100);
    if (result == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0, got %u)\n", result);

    /* TEST 2: Normal case, timeout elapsed: start=100, now=250, timeout=100 -> 1 */
    result = timeout_elapsed(100, 250, 100);
    if (result == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 1, got %u)\n", result);

    /* TEST 3: Wraparound case: start=0xFFFFFFF0, now=0x00000010, timeout=0x30 -> 1 */
    /* elapsed = 0x10 - 0xFFFFFFF0 = 0x20, timeout=0x30 -> not elapsed */
    result = timeout_elapsed(0xFFFFFFF0, 0x00000010, 0x30);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: Wraparound case: start=0xFFFFFFF0, now=0x00000030, timeout=0x30 -> 1 */
    /* elapsed = 0x30 - 0xFFFFFFF0 = 0x40, timeout=0x30 -> elapsed */
    result = timeout_elapsed(0xFFFFFFF0, 0x00000030, 0x30);
    if (result == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 1, got %u)\n", result);

    return 0;
}
