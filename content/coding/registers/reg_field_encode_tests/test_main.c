#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Single field - 3-bit value 0x5 at bit 4 => 0x00000050 */
    uint8_t f1[] = {4, 3, 0x05};
    uint32_t result = encode_register(f1, 1);
    if (result == 0x00000050) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00000050, got 0x%08X)\n", result);

    /* TEST 2: Two fields - 4-bit val 0xA at bit 0, 4-bit val 0x3 at bit 4 => 0x0000003A */
    uint8_t f2[] = {0, 4, 0x0A, 4, 4, 0x03};
    result = encode_register(f2, 2);
    if (result == 0x0000003A) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x0000003A, got 0x%08X)\n", result);

    /* TEST 3: Value exceeds width - 2-bit field with value 0xFF should mask to 0x3 at bit 8 => 0x00000300 */
    uint8_t f3[] = {8, 2, 0xFF};
    result = encode_register(f3, 1);
    if (result == 0x00000300) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x00000300, got 0x%08X)\n", result);

    /* TEST 4: Three fields combined - 8-bit 0xDE at bit 24, 8-bit 0xAD at bit 16, 8-bit 0xBE at bit 8 */
    uint8_t f4[] = {24, 8, 0xDE, 16, 8, 0xAD, 8, 8, 0xBE};
    result = encode_register(f4, 3);
    if (result == 0xDEADBE00) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xDEADBE00, got 0x%08X)\n", result);

    return 0;
}
