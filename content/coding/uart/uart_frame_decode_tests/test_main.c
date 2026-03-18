#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t result;

    /* TEST 1: Valid frame for 0x55: frame=0x04AA -> data=0x55 */
    result = uart_decode_frame(0x04AA);
    if (result == 0x55) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x55, got 0x%02X)\n", result);

    /* TEST 2: Valid frame for 0x01: frame=0x0602 -> data=0x01 */
    result = uart_decode_frame(0x0602);
    if (result == 0x01) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x01, got 0x%02X)\n", result);

    /* TEST 3: Invalid start bit (bit 0 = 1): frame=0x04AB -> error 0xFF */
    result = uart_decode_frame(0x04AB);
    if (result == 0xFF) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0xFF, got 0x%02X)\n", result);

    /* TEST 4: Invalid stop bit (bit 10 = 0): frame=0x00AA -> error 0xFF */
    result = uart_decode_frame(0x00AA);
    if (result == 0xFF) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xFF, got 0x%02X)\n", result);

    return 0;
}
