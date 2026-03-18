#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint16_t result;

    /* TEST 1: data=0x55, even parity.
       0x55 = 01010101 -> 4 ones (even) -> parity bit = 0.
       Frame: start(0) | data(0xAA shifted) | parity(0) | stop(1)
       Bits: 0 | 10101010 | 0 | 1 = 0b10_0_10101010_0 = 0x04AA */
    result = uart_build_frame(0x55, 1);
    if (result == 0x04AA) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x04AA, got 0x%04X)\n", result);

    /* TEST 2: data=0x03, odd parity.
       0x03 = 00000011 -> 2 ones (even) -> odd parity bit = 1.
       Frame: 0 | 00000110 | 1 | 1 = 0b10_1_00000110_0 = 0x0606
       Bits[10:0]: 1_1_00000110_0
       = (1<<10)|(1<<9)|(0x03<<1) = 0x400|0x200|0x06 = 0x0606 */
    result = uart_build_frame(0x03, 2);
    if (result == 0x0606) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x0606, got 0x%04X)\n", result);

    /* TEST 3: data=0xFF, no parity.
       No parity -> stop bit at bit 9.
       Frame: 0 | 11111111 | 1(stop) = (1<<9)|(0xFF<<1) = 0x200|0x1FE = 0x03FE */
    result = uart_build_frame(0xFF, 0);
    if (result == 0x03FE) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x03FE, got 0x%04X)\n", result);

    /* TEST 4: data=0x00, even parity.
       0 ones -> even -> parity bit = 0.
       Frame: 0 | 00000000 | 0 | 1 = (1<<10) = 0x0400 */
    result = uart_build_frame(0x00, 1);
    if (result == 0x0400) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x0400, got 0x%04X)\n", result);

    return 0;
}
