#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    Mutex m;

    /* TEST 1: Lock an unlocked mutex, verify owner */
    mutex_init(&m);
    int r = mutex_lock(&m, 3);
    if (r == 1 && mutex_get_owner(&m) == 3) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected r=1 owner=3, got r=%d owner=%u)\n", r, mutex_get_owner(&m));

    /* TEST 2: Another task tries to lock - should fail */
    r = mutex_try_lock(&m, 5);
    if (r == 0 && mutex_get_owner(&m) == 3) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected r=0 owner=3, got r=%d owner=%u)\n", r, mutex_get_owner(&m));

    /* TEST 3: Wrong task tries to unlock - should fail */
    r = mutex_unlock(&m, 5);
    if (r == 0 && mutex_get_owner(&m) == 3) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected r=0 owner=3, got r=%d owner=%u)\n", r, mutex_get_owner(&m));

    /* TEST 4: Correct owner unlocks, then new task acquires */
    r = mutex_unlock(&m, 3);
    int r2 = mutex_lock(&m, 7);
    if (r == 1 && r2 == 1 && mutex_get_owner(&m) == 7) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected r=1 r2=1 owner=7, got r=%d r2=%d owner=%u)\n", r, r2, mutex_get_owner(&m));

    return 0;
}
