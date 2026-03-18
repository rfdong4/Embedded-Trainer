#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Read pins 1 and 3 from 0x0A (both high) -> 0x03 */
    {
        uint8_t pins[] = {1, 3};
        result = read_pins(0x0000000A, pins, 2);
        if (result == 0x03) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0x03, got 0x%02X)\n", result);
    }

    /* TEST 2: Read pins 0,1,2,3 from 0x05 (pins 0 and 2 high) -> 0x05 */
    {
        uint8_t pins[] = {0, 1, 2, 3};
        result = read_pins(0x00000005, pins, 4);
        if (result == 0x05) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 0x05, got 0x%02X)\n", result);
    }

    /* TEST 3: Read pin 7 from 0x80 (pin 7 high) -> 0x01 */
    {
        uint8_t pins[] = {7};
        result = read_pins(0x00000080, pins, 1);
        if (result == 0x01) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0x01, got 0x%02X)\n", result);
    }

    /* TEST 4: Read pins 0,4,8 from 0x00 (all low) -> 0x00 */
    {
        uint8_t pins[] = {0, 4, 8};
        result = read_pins(0x00000000, pins, 3);
        if (result == 0x00) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 0x00, got 0x%02X)\n", result);
    }

    return 0;
}
