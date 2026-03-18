#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    Semaphore sem;
    uint8_t result;

    /* TEST 1: Wait on semaphore with count > 0 succeeds */
    sem_init(&sem, 3, 5);
    result = sem_wait(&sem);
    if (result == 1 && sem.count == 2) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected result=1 count=2, got result=%u count=%u)\n", result, sem.count);

    /* TEST 2: Wait on empty semaphore fails */
    sem_init(&sem, 0, 5);
    result = sem_wait(&sem);
    if (result == 0 && sem.count == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected result=0 count=0, got result=%u count=%u)\n", result, sem.count);

    /* TEST 3: Signal increments count */
    sem_init(&sem, 1, 3);
    result = sem_signal(&sem);
    if (result == 1 && sem.count == 2) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected result=1 count=2, got result=%u count=%u)\n", result, sem.count);

    /* TEST 4: Signal on full semaphore fails */
    sem_init(&sem, 3, 3);
    result = sem_signal(&sem);
    if (result == 0 && sem.count == 3) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected result=0 count=3, got result=%u count=%u)\n", result, sem.count);

    return 0;
}
