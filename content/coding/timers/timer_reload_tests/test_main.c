#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint16_t result;

    /* TEST 1: 16MHz, prescaler=16, 1000us -> (16M/16)*1000/1M = 1000 */
    result = timer_reload(16000000, 16, 1000);
    if (result == 1000) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1000, got %u)\n", result);

    /* TEST 2: 8MHz, prescaler=8, 500us -> (8M/8)*500/1M = 500 */
    result = timer_reload(8000000, 8, 500);
    if (result == 500) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 500, got %u)\n", result);

    /* TEST 3: 72MHz, prescaler=72, 100us -> (72M/72)*100/1M = 100 */
    result = timer_reload(72000000, 72, 100);
    if (result == 100) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 100, got %u)\n", result);

    /* TEST 4: 48MHz, prescaler=48, 10000us -> (48M/48)*10000/1M = 10000 */
    result = timer_reload(48000000, 48, 10000);
    if (result == 10000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 10000, got %u)\n", result);

    return 0;
}
