#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t output[8];

    /* TEST 1: XOR with zero context, rotation at index 0 = no rotation
       master=0xAA, context=0x00 -> XOR=0xAA, rot 0 -> 0xAA */
    uint8_t mk1[] = {0xAA};
    uint8_t cx1[] = {0x00};
    derive_key(mk1, cx1, output, 1);
    if (output[0] == 0xAA) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0xAA, got 0x%02X)\n", output[0]);

    /* TEST 2: XOR and rotate at index 1
       master=0x00, context=0x81 -> XOR=0x81, rot 1 -> (0x81<<1)|(0x81>>7) = 0x02|0x01 = 0x03 */
    uint8_t mk2[] = {0x00, 0x00};
    uint8_t cx2[] = {0x00, 0x81};
    derive_key(mk2, cx2, output, 2);
    if (output[1] == 0x03) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x03, got 0x%02X)\n", output[1]);

    /* TEST 3: 4-byte derivation, check all bytes
       mk={0xFF,0x0F,0xAA,0x55}, cx={0x0F,0xF0,0x55,0xAA}
       i=0: XOR=0xF0, rot 0 -> 0xF0
       i=1: XOR=0xFF, rot 1 -> (0xFF<<1)|(0xFF>>7) = 0xFE|0x01 = 0xFF
       i=2: XOR=0xFF, rot 2 -> (0xFF<<2)|(0xFF>>6) = 0xFC|0x03 = 0xFF
       i=3: XOR=0xFF, rot 3 -> (0xFF<<3)|(0xFF>>5) = 0xF8|0x07 = 0xFF */
    uint8_t mk3[] = {0xFF, 0x0F, 0xAA, 0x55};
    uint8_t cx3[] = {0x0F, 0xF0, 0x55, 0xAA};
    derive_key(mk3, cx3, output, 4);
    if (output[0] == 0xF0 && output[1] == 0xFF && output[2] == 0xFF && output[3] == 0xFF)
        printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (got 0x%02X 0x%02X 0x%02X 0x%02X)\n",
                output[0], output[1], output[2], output[3]);

    /* TEST 4: Identity - XOR with self gives 0, rotation of 0 is 0 */
    uint8_t mk4[] = {0x12, 0x34, 0x56, 0x78};
    uint8_t cx4[] = {0x12, 0x34, 0x56, 0x78};
    derive_key(mk4, cx4, output, 4);
    if (output[0] == 0x00 && output[1] == 0x00 && output[2] == 0x00 && output[3] == 0x00)
        printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (got 0x%02X 0x%02X 0x%02X 0x%02X)\n",
                output[0], output[1], output[2], output[3]);

    return 0;
}
