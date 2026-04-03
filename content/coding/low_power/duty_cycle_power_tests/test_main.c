#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: 50ms active at 15mA, 9950ms sleep at 3uA
       = (50*15000 + 9950*3) / 10000 = (750000 + 29850) / 10000 = 77 */
    result = calc_avg_current(50, 9950, 15000, 3);
    if (result == 77) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 77, got %u)\n", result);

    /* TEST 2: Always active (no sleep) => just active current */
    result = calc_avg_current(1000, 0, 5000, 0);
    if (result == 5000) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 5000, got %u)\n", result);

    /* TEST 3: Total time zero => return 0 */
    result = calc_avg_current(0, 0, 5000, 2);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: 100ms active at 20mA, 900ms sleep at 10uA
       = (100*20000 + 900*10) / 1000 = (2000000 + 9000) / 1000 = 2009 */
    result = calc_avg_current(100, 900, 20000, 10);
    if (result == 2009) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 2009, got %u)\n", result);

    return 0;
}
