#include <stdio.h>
#include <stdint.h>
#include "solution.c"

/* Reference implementation to generate expected values */
static uint16_t crc16_ref(const uint8_t *data, uint32_t len) {
    uint16_t crc = 0xFFFF;
    for (uint32_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc = crc << 1;
            crc &= 0xFFFF;
        }
    }
    return crc;
}

int main(void) {
    /* TEST 1: CRC of "123456789" = 0x29B1 (standard check value for CRC-16/CCITT-FALSE) */
    uint8_t d1[] = {'1','2','3','4','5','6','7','8','9'};
    uint16_t r1 = crc16(d1, 9);
    if (r1 == 0x29B1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x29B1, got 0x%04X)\n", r1);

    /* TEST 2: CRC of single byte 0x00 */
    uint8_t d2[] = {0x00};
    uint16_t r2 = crc16(d2, 1);
    uint16_t exp2 = crc16_ref(d2, 1);
    if (r2 == exp2) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x%04X, got 0x%04X)\n", exp2, r2);

    /* TEST 3: CRC of empty data (length 0) should return initial value 0xFFFF */
    uint8_t dummy = 0;
    uint16_t r3 = crc16(&dummy, 0);
    if (r3 == 0xFFFF) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0xFFFF, got 0x%04X)\n", r3);

    /* TEST 4: CRC of {0xDE, 0xAD} */
    uint8_t d4[] = {0xDE, 0xAD};
    uint16_t r4 = crc16(d4, 2);
    uint16_t exp4 = crc16_ref(d4, 2);
    if (r4 == exp4) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x%04X, got 0x%04X)\n", exp4, r4);

    return 0;
}
