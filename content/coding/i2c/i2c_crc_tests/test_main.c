#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: {0xA0, 0x00, 0x55} -> 0xA0^0x00^0x55 = 0xF5 */
    {
        uint8_t data[] = {0xA0, 0x00, 0x55};
        result = i2c_pec(data, 3);
        if (result == 0xF5) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0xF5, got 0x%02X)\n", result);
    }

    /* TEST 2: {0xFF, 0xFF} -> 0xFF^0xFF = 0x00 */
    {
        uint8_t data[] = {0xFF, 0xFF};
        result = i2c_pec(data, 2);
        if (result == 0x00) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 0x00, got 0x%02X)\n", result);
    }

    /* TEST 3: Single byte {0x42} -> 0x42 */
    {
        uint8_t data[] = {0x42};
        result = i2c_pec(data, 1);
        if (result == 0x42) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0x42, got 0x%02X)\n", result);
    }

    /* TEST 4: {0x01, 0x02, 0x04, 0x08} -> 0x01^0x02^0x04^0x08 = 0x0F */
    {
        uint8_t data[] = {0x01, 0x02, 0x04, 0x08};
        result = i2c_pec(data, 4);
        if (result == 0x0F) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 0x0F, got 0x%02X)\n", result);
    }

    return 0;
}
