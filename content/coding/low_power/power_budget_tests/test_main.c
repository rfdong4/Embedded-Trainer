#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: All components enabled */
    component_t sys1[] = {
        {8000, 1},   /* MCU: 8 mA */
        {12000, 1},  /* Radio: 12 mA */
        {500, 1},    /* Sensor: 0.5 mA */
        {50, 1}      /* Regulator: 50 uA */
    };
    result = calc_power_budget(sys1, 4);
    if (result == 20550) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 20550, got %u)\n", result);

    /* TEST 2: Only MCU enabled (sleep mode) */
    component_t sys2[] = {
        {2, 1},      /* MCU sleep: 2 uA */
        {0, 0},      /* Radio off */
        {0, 0},      /* Sensor off */
        {50, 1}      /* Regulator quiescent */
    };
    result = calc_power_budget(sys2, 4);
    if (result == 52) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 52, got %u)\n", result);

    /* TEST 3: Empty system */
    result = calc_power_budget(sys1, 0);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: Mixed enabled/disabled */
    component_t sys4[] = {
        {1000, 1},
        {2000, 0},
        {3000, 1},
        {4000, 0},
        {5000, 1}
    };
    result = calc_power_budget(sys4, 5);
    if (result == 9000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 9000, got %u)\n", result);

    return 0;
}
