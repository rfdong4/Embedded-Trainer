#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: 0x00 has 0 ones (even) -> parity = 0 */
    result = even_parity(0x00);
    if (result == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0, got %u)\n", result);

    /* TEST 2: 0x01 has 1 one (odd) -> parity = 1 */
    result = even_parity(0x01);
    if (result == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 1, got %u)\n", result);

    /* TEST 3: 0xFF has 8 ones (even) -> parity = 0 */
    result = even_parity(0xFF);
    if (result == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %u)\n", result);

    /* TEST 4: 0xA5 = 10100101 has 4 ones (even) -> parity = 0 */
    result = even_parity(0xA5);
    if (result == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0, got %u)\n", result);

    return 0;
}
