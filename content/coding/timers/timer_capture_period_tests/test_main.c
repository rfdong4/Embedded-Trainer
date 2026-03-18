#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: No wrap: cap1=1000, cap2=2000, freq=1MHz -> 1000 us */
    result = capture_period_us(1000, 2000, 1000000);
    if (result == 1000) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1000, got %u)\n", result);

    /* TEST 2: Wrap-around: cap1=60000, cap2=5000, freq=1MHz -> 10536 us */
    result = capture_period_us(60000, 5000, 1000000);
    if (result == 10536) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 10536, got %u)\n", result);

    /* TEST 3: No wrap: cap1=0, cap2=50000, freq=2MHz -> 25000 us */
    result = capture_period_us(0, 50000, 2000000);
    if (result == 25000) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 25000, got %u)\n", result);

    /* TEST 4: Wrap-around: cap1=40000, cap2=10000, freq=500kHz -> 71072 us */
    result = capture_period_us(40000, 10000, 500000);
    if (result == 71072) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 71072, got %u)\n", result);

    return 0;
}
