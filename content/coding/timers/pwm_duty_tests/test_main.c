#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint16_t result;

    /* TEST 1: period=1000, 50% duty -> 500 */
    result = pwm_compare(1000, 50);
    if (result == 500) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 500, got %u)\n", result);

    /* TEST 2: period=1000, 0% duty -> 0 */
    result = pwm_compare(1000, 0);
    if (result == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0, got %u)\n", result);

    /* TEST 3: period=1000, 100% duty -> 1000 */
    result = pwm_compare(1000, 100);
    if (result == 1000) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 1000, got %u)\n", result);

    /* TEST 4: period=65535, 75% duty -> 49151 */
    result = pwm_compare(65535, 75);
    if (result == 49151) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 49151, got %u)\n", result);

    return 0;
}
