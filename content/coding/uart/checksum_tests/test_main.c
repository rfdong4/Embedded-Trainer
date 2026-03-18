#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: {0x01, 0x02, 0x03} -> 0x06 */
    {
        uint8_t data[] = {0x01, 0x02, 0x03};
        result = checksum(data, 3);
        if (result == 0x06) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0x06, got 0x%02X)\n", result);
    }

    /* TEST 2: {0xFF, 0x01} -> 0x00 (overflow wraps) */
    {
        uint8_t data[] = {0xFF, 0x01};
        result = checksum(data, 2);
        if (result == 0x00) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 0x00, got 0x%02X)\n", result);
    }

    /* TEST 3: {0x48, 0x65, 0x6C, 0x6C, 0x6F} "Hello" -> 0xF4 */
    {
        uint8_t data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
        result = checksum(data, 5);
        if (result == 0xF4) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0xF4, got 0x%02X)\n", result);
    }

    /* TEST 4: Single byte {0xAB} -> 0xAB */
    {
        uint8_t data[] = {0xAB};
        result = checksum(data, 1);
        if (result == 0xAB) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 0xAB, got 0x%02X)\n", result);
    }

    return 0;
}
