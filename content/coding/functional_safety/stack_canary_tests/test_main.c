#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t canary_location;

    /* TEST 1: Init sets canary value */
    stack_canary_init(&canary_location);
    if (canary_location == 0xDEADBEEF) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0xDEADBEEF, got 0x%08X)\n", canary_location);

    /* TEST 2: Check returns 1 when canary is intact */
    {
        uint8_t result = stack_canary_check(&canary_location);
        if (result == 1) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 1, got %u)\n", result);
    }

    /* TEST 3: Check returns 0 when canary is corrupted */
    {
        canary_location = 0x12345678;  /* Simulate corruption */
        uint8_t result = stack_canary_check(&canary_location);
        if (result == 0) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0, got %u)\n", result);
    }

    /* TEST 4: Re-init restores canary, check passes again */
    {
        stack_canary_init(&canary_location);
        uint8_t result = stack_canary_check(&canary_location);
        if (result == 1) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 1, got %u)\n", result);
    }

    return 0;
}
