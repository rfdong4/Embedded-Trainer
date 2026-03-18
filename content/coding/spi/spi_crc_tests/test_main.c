#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: CRC of {0xBE, 0xEF} = 0x92 */
    uint8_t data1[] = {0xBE, 0xEF};
    result = spi_crc8(data1, 2);
    if (result == 0x92) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x92, got 0x%02X)\n", result);

    /* TEST 2: CRC of single byte {0x00} = 0xAC */
    uint8_t data2[] = {0x00};
    result = spi_crc8(data2, 1);
    if (result == 0xAC) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0xAC, got 0x%02X)\n", result);

    /* TEST 3: CRC of {0xFF} = 0x00 */
    uint8_t data3[] = {0xFF};
    result = spi_crc8(data3, 1);
    if (result == 0x00) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x00, got 0x%02X)\n", result);

    /* TEST 4: CRC of {0x01, 0x02, 0x03, 0x04} = 0x29 */
    uint8_t data4[] = {0x01, 0x02, 0x03, 0x04};
    result = spi_crc8(data4, 4);
    if (result == 0x29) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x29, got 0x%02X)\n", result);

    return 0;
}
