#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: 16MHz sys, 8MHz max -> divider=2 (16/2=8) */
    result = spi_clock_div(16000000, 8000000);
    if (result == 2) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 2, got %u)\n", result);

    /* TEST 2: 48MHz sys, 12MHz max -> divider=4 (48/4=12) */
    result = spi_clock_div(48000000, 12000000);
    if (result == 4) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 4, got %u)\n", result);

    /* TEST 3: 72MHz sys, 1MHz max -> divider=128 (72/128=0.5625MHz) */
    result = spi_clock_div(72000000, 1000000);
    if (result == 128) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 128, got %u)\n", result);

    /* TEST 4: 8MHz sys, 8MHz max -> divider=1 (8/1=8) */
    result = spi_clock_div(8000000, 8000000);
    if (result == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 1, got %u)\n", result);

    return 0;
}
