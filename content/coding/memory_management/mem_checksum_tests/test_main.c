#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: CRC-8 of a single byte 0x31 */
    {
        uint8_t data[] = {0x31};
        uint8_t crc = crc8_calc(data, 1);
        /* CRC-8 of 0x31 with poly 0x07, init 0x00: 0x97 */
        if (crc == 0x97)
            printf("TEST 1: PASS\n");
        else
            printf("TEST 1: FAIL (expected 0x97, got 0x%02X)\n", crc);
    }

    /* TEST 2: CRC-8 of "123456789" (standard test vector) */
    {
        uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
        uint8_t crc = crc8_calc(data, 9);
        /* Standard CRC-8 check value for "123456789" is 0xF4 */
        if (crc == 0xF4)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (expected 0xF4, got 0x%02X)\n", crc);
    }

    /* TEST 3: Verify function returns 1 for correct checksum */
    {
        uint8_t data[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
        int result = crc8_verify(data, 9, 0xF4);
        if (result == 1)
            printf("TEST 3: PASS\n");
        else
            printf("TEST 3: FAIL (expected verify=1, got %d)\n", result);
    }

    /* TEST 4: Verify function returns 0 for incorrect checksum */
    {
        uint8_t data[] = {0xAA, 0xBB, 0xCC};
        uint8_t correct_crc = crc8_calc(data, 3);
        int result = crc8_verify(data, 3, (uint8_t)(correct_crc + 1));
        if (result == 0)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (expected verify=0, got %d)\n", result);
    }

    return 0;
}
