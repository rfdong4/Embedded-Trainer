#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Lock an unlocked mutex succeeds */
    mutex_init();
    int result = mutex_lock();
    if (result == 0 && mutex_is_locked() == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected result=0 locked=1, got result=%d locked=%d)\n", result, mutex_is_locked());

    /* TEST 2: Locking an already-locked mutex fails */
    result = mutex_lock();
    if (result == -1 && mutex_is_locked() == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected result=-1 locked=1, got result=%d locked=%d)\n", result, mutex_is_locked());

    /* TEST 3: Unlock a locked mutex succeeds */
    result = mutex_unlock();
    if (result == 0 && mutex_is_locked() == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected result=0 locked=0, got result=%d locked=%d)\n", result, mutex_is_locked());

    /* TEST 4: Unlocking an already-unlocked mutex fails */
    result = mutex_unlock();
    if (result == -1 && mutex_is_locked() == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected result=-1 locked=0, got result=%d locked=%d)\n", result, mutex_is_locked());

    return 0;
}
