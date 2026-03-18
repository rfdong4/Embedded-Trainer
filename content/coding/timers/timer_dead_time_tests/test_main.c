#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;
    uint16_t ch1, ch2;

    /* TEST 1: period=1000, duty=500(50%), dead=10 -> on=500, ch1=490, ch2=510 */
    result = calc_dead_time_pwm(1000, 500, 10);
    ch1 = (uint16_t)(result >> 16);
    ch2 = (uint16_t)(result & 0xFFFF);
    if (ch1 == 490 && ch2 == 510) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected ch1=490 ch2=510, got ch1=%u ch2=%u)\n", ch1, ch2);

    /* TEST 2: period=2000, duty=250(25%), dead=20 -> on=500, ch1=480, ch2=520 */
    result = calc_dead_time_pwm(2000, 250, 20);
    ch1 = (uint16_t)(result >> 16);
    ch2 = (uint16_t)(result & 0xFFFF);
    if (ch1 == 480 && ch2 == 520) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected ch1=480 ch2=520, got ch1=%u ch2=%u)\n", ch1, ch2);

    /* TEST 3: period=1000, duty=750(75%), dead=5 -> on=750, ch1=745, ch2=755 */
    result = calc_dead_time_pwm(1000, 750, 5);
    ch1 = (uint16_t)(result >> 16);
    ch2 = (uint16_t)(result & 0xFFFF);
    if (ch1 == 745 && ch2 == 755) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected ch1=745 ch2=755, got ch1=%u ch2=%u)\n", ch1, ch2);

    /* TEST 4: period=4000, duty=100(10%), dead=50 -> on=400, ch1=350, ch2=450 */
    result = calc_dead_time_pwm(4000, 100, 50);
    ch1 = (uint16_t)(result >> 16);
    ch2 = (uint16_t)(result & 0xFFFF);
    if (ch1 == 350 && ch2 == 450) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected ch1=350 ch2=450, got ch1=%u ch2=%u)\n", ch1, ch2);

    return 0;
}
