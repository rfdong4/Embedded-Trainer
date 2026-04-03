#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    PIMutex m = {0, 0, 0, 0};
    uint8_t tasks[3][2] = {{0, 3}, {1, 1}, {2, 2}};  /* id, prio */

    /* TEST 1: Low-priority task (prio=3) acquires mutex */
    {
        uint8_t r = mutex_lock(&m, 0, 3);
        if (r == 1 && m.held == 1 && m.owner == 0) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected acquired, got r=%u)\n", r);
    }

    /* TEST 2: High-priority task (prio=1) tries to lock — blocked, holder boosted */
    {
        uint8_t r = mutex_lock(&m, 1, 1);
        uint8_t eff = get_effective_priority(&m, tasks, 0, 3);
        if (r == 0 && eff == 1) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected blocked+boost to 1, got r=%u eff=%u)\n", r, eff);
    }

    /* TEST 3: Unlock restores original priority */
    {
        mutex_unlock(&m, tasks, 3);
        uint8_t eff = get_effective_priority(&m, tasks, 0, 3);
        if (m.held == 0 && eff == 3) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected prio restored to 3, got eff=%u)\n", eff);
    }

    /* TEST 4: Medium-priority task acquires, no boost needed */
    {
        uint8_t r = mutex_lock(&m, 2, 2);
        uint8_t eff = get_effective_priority(&m, tasks, 2, 3);
        if (r == 1 && eff == 2) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected acquired at prio 2, got r=%u eff=%u)\n", r, eff);
    }

    return 0;
}
