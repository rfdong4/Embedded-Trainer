#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint16_t result;

    /* TEST 1: 16MHz clock, 9600 baud -> 16000000/(16*9600) = 104 */
    result = baud_divisor(16000000, 9600);
    if (result == 104) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 104, got %u)\n", result);

    /* TEST 2: 48MHz clock, 115200 baud -> 48000000/(16*115200) = 26 */
    result = baud_divisor(48000000, 115200);
    if (result == 26) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 26, got %u)\n", result);

    /* TEST 3: 8MHz clock, 9600 baud -> 8000000/(16*9600) = 52 */
    result = baud_divisor(8000000, 9600);
    if (result == 52) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 52, got %u)\n", result);

    /* TEST 4: 72MHz clock, 38400 baud -> 72000000/(16*38400) = 117 */
    result = baud_divisor(72000000, 38400);
    if (result == 117) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 117, got %u)\n", result);

    return 0;
}
