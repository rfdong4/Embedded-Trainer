#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: All active, current=0, num=4 -> next=1 */
    {
        uint8_t active[] = {1, 1, 1, 1};
        result = next_task(0, active, 4);
        if (result == 1) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 1, got %u)\n", result);
    }

    /* TEST 2: Skip inactive: current=0, task 1 inactive -> next=2 */
    {
        uint8_t active[] = {1, 0, 1, 1};
        result = next_task(0, active, 4);
        if (result == 2) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 2, got %u)\n", result);
    }

    /* TEST 3: Wraparound: current=3, all active -> next=0 */
    {
        uint8_t active[] = {1, 1, 1, 1};
        result = next_task(3, active, 4);
        if (result == 0) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0, got %u)\n", result);
    }

    /* TEST 4: No other active tasks -> return current */
    {
        uint8_t active[] = {0, 0, 1, 0};
        result = next_task(2, active, 4);
        if (result == 2) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 2, got %u)\n", result);
    }

    return 0;
}
