#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Average of {-40, -50, -60, -50} = -200/4 = -50 */
    int8_t s1[] = {-40, -50, -60, -50};
    int8_t r1 = rssi_average(s1, 4);
    if (r1 == -50) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected -50, got %d)\n", r1);

    /* TEST 2: Single sample {-75} -> -75 */
    int8_t s2[] = {-75};
    int8_t r2 = rssi_average(s2, 1);
    if (r2 == -75) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected -75, got %d)\n", r2);

    /* TEST 3: Average of {-80, -70} = -150/2 = -75 */
    int8_t s3[] = {-80, -70};
    int8_t r3 = rssi_average(s3, 2);
    if (r3 == -75) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected -75, got %d)\n", r3);

    /* TEST 4: Average of {-30, -40, -50} = -120/3 = -40 */
    int8_t s4[] = {-30, -40, -50};
    int8_t r4 = rssi_average(s4, 3);
    if (r4 == -40) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected -40, got %d)\n", r4);

    return 0;
}
