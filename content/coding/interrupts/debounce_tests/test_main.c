#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: All last 3 samples are 1 -> 1 */
    {
        uint8_t samples[] = {0, 0, 1, 1, 1};
        result = debounce(samples, 5, 3);
        if (result == 1) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 1, got %u)\n", result);
    }

    /* TEST 2: Last sample is 0, not stable -> 0 */
    {
        uint8_t samples[] = {1, 1, 1, 1, 0};
        result = debounce(samples, 5, 3);
        if (result == 0) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 0, got %u)\n", result);
    }

    /* TEST 3: Mixed samples, not enough consecutive 1s -> 0 */
    {
        uint8_t samples[] = {1, 0, 1, 0, 1};
        result = debounce(samples, 5, 3);
        if (result == 0) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0, got %u)\n", result);
    }

    /* TEST 4: Exactly required samples all 1 -> 1 */
    {
        uint8_t samples[] = {1, 1, 1, 1};
        result = debounce(samples, 4, 4);
        if (result == 1) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 1, got %u)\n", result);
    }

    return 0;
}
