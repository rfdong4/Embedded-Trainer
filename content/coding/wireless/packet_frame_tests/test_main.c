#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t frame[264];

    /* TEST 1: Single byte payload {0x42}
       Frame: AA 7E 01 42 00 42  -> length=6 */
    uint8_t p1[] = {0x42};
    uint32_t len1 = build_frame(frame, p1, 1);
    if (len1 == 6 && frame[0] == 0xAA && frame[1] == 0x7E && frame[2] == 0x01 &&
        frame[3] == 0x42 && frame[4] == 0x00 && frame[5] == 0x42)
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (len=%u, bytes=%02X %02X %02X %02X %02X %02X)\n",
               len1, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5]);

    /* TEST 2: Two byte payload {0x80, 0x81} -> checksum = 0x0101
       Frame: AA 7E 02 80 81 01 01  -> length=7 */
    uint8_t p2[] = {0x80, 0x81};
    uint32_t len2 = build_frame(frame, p2, 2);
    if (len2 == 7 && frame[3] == 0x80 && frame[4] == 0x81 &&
        frame[5] == 0x01 && frame[6] == 0x01)
        printf("TEST 2: PASS\n");
    else
        printf("TEST 2: FAIL (len=%u, cs=%02X%02X)\n", len2, frame[5], frame[6]);

    /* TEST 3: Empty payload -> checksum = 0x0000
       Frame: AA 7E 00 00 00  -> length=5 */
    uint32_t len3 = build_frame(frame, NULL, 0);
    if (len3 == 5 && frame[0] == 0xAA && frame[1] == 0x7E && frame[2] == 0x00 &&
        frame[3] == 0x00 && frame[4] == 0x00)
        printf("TEST 3: PASS\n");
    else
        printf("TEST 3: FAIL (len=%u)\n", len3);

    /* TEST 4: Three byte payload {0xFF, 0xFF, 0x01} -> checksum = 0x01FF
       Frame: AA 7E 03 FF FF 01 01 FF -> length=8 */
    uint8_t p4[] = {0xFF, 0xFF, 0x01};
    uint32_t len4 = build_frame(frame, p4, 3);
    if (len4 == 8 && frame[3] == 0xFF && frame[4] == 0xFF && frame[5] == 0x01 &&
        frame[6] == 0x01 && frame[7] == 0xFF)
        printf("TEST 4: PASS\n");
    else
        printf("TEST 4: FAIL (len=%u, cs=%02X%02X)\n", len4, frame[6], frame[7]);

    return 0;
}
