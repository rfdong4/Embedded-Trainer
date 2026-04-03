#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;
    uint8_t status;

    /* TEST 1: All three agree */
    result = tmr_vote_with_status(42, 42, 42, &status);
    if (result == 42 && status == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected result=42 status=0, got result=%u status=%u)\n", result, status);

    /* TEST 2: Two agree, one disagrees (c is faulty) */
    result = tmr_vote_with_status(100, 100, 999, &status);
    if (result == 100 && status == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected result=100 status=1, got result=%u status=%u)\n", result, status);

    /* TEST 3: Two agree, first one disagrees (a is faulty) */
    result = tmr_vote_with_status(999, 55, 55, &status);
    if (result == 55 && status == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected result=55 status=1, got result=%u status=%u)\n", result, status);

    /* TEST 4: All three disagree */
    result = tmr_vote_with_status(1, 2, 3, &status);
    if (status == 2) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected status=2, got status=%u)\n", status);

    return 0;
}
