#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "solution.c"

int main(void) {
    uint32_t crc;

    /* TEST 1: CRC-32 of "123456789" should be 0xCBF43926 */
    {
        const uint8_t data[] = "123456789";
        crc = crc32_calc(data, 9);
        if (crc == 0xCBF43926) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0xCBF43926, got 0x%08X)\n", crc);
    }

    /* TEST 2: CRC of single byte 0x00 */
    {
        const uint8_t data[] = {0x00};
        crc = crc32_calc(data, 1);
        if (crc == 0xD202EF8D) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 0xD202EF8D, got 0x%08X)\n", crc);
    }

    /* TEST 3: CRC of empty data (length 0) should be 0x00000000 */
    {
        crc = crc32_calc(NULL, 0);
        if (crc == 0x00000000) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 0x00000000, got 0x%08X)\n", crc);
    }

    /* TEST 4: CRC of known pattern {0xDE, 0xAD, 0xBE, 0xEF} */
    {
        const uint8_t data[] = {0xDE, 0xAD, 0xBE, 0xEF};
        crc = crc32_calc(data, 4);
        /* Pre-computed: CRC32 of DEADBEEF bytes */
        if (crc == 0xFA4B7AB0) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 0xFA4B7AB0, got 0x%08X)\n", crc);
    }

    return 0;
}
