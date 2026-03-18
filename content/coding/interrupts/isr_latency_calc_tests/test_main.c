#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Simple (non-nested) ISR latency */
    {
        uint32_t lat = calc_isr_latency(12, 50, 10, 8, 0);
        /* 12 + 50 + 10 = 72 */
        if (lat == 72)
            printf("TEST 1: PASS\n");
        else
            printf("TEST 1: FAIL (expected 72, got %u)\n", lat);
    }

    /* TEST 2: Nested ISR latency includes preemption cost */
    {
        uint32_t lat = calc_isr_latency(12, 50, 10, 8, 1);
        /* 12 + 8 + 50 + 10 = 80 */
        if (lat == 80)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (expected 80, got %u)\n", lat);
    }

    /* TEST 3: Worst-case across multiple ISRs */
    {
        ISR_Timing isrs[] = {
            {30, 0},   /* 12 + 30 + 10 = 52 */
            {100, 1},  /* 12 + 8 + 100 + 10 = 130 */
            {80, 0},   /* 12 + 80 + 10 = 102 */
        };
        uint32_t worst = calc_worst_case_latency(isrs, 3, 12, 10, 8);
        if (worst == 130)
            printf("TEST 3: PASS\n");
        else
            printf("TEST 3: FAIL (expected 130, got %u)\n", worst);
    }

    /* TEST 4: Worst-case with all nested ISRs */
    {
        ISR_Timing isrs[] = {
            {20, 1},   /* 6 + 4 + 20 + 6 = 36 */
            {45, 1},   /* 6 + 4 + 45 + 6 = 61 */
            {10, 1},   /* 6 + 4 + 10 + 6 = 26 */
            {44, 1},   /* 6 + 4 + 44 + 6 = 60 */
        };
        uint32_t worst = calc_worst_case_latency(isrs, 4, 6, 6, 4);
        if (worst == 61)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (expected 61, got %u)\n", worst);
    }

    return 0;
}
