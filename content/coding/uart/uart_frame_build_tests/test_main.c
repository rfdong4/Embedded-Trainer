#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint16_t result;

    /* TEST 1: data=0x55 (01010101), 4 ones -> parity=0, frame=0|0xAA|0|0x400=0x4AA */
    result = uart_build_frame(0x55);
    if (result == 0x04AA) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x04AA, got 0x%04X)\n", result);

    /* TEST 2: data=0xFF (11111111), 8 ones -> parity=0, frame=0|0x1FE|0|0x400=0x5FE */
    result = uart_build_frame(0xFF);
    if (result == 0x05FE) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x05FE, got 0x%04X)\n", result);

    /* TEST 3: data=0x01 (00000001), 1 one -> parity=1, frame=0|0x02|0x200|0x400=0x602 */
    result = uart_build_frame(0x01);
    if (result == 0x0602) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x0602, got 0x%04X)\n", result);

    /* TEST 4: data=0x00 (00000000), 0 ones -> parity=0, frame=0|0|0|0x400=0x400 */
    result = uart_build_frame(0x00);
    if (result == 0x0400) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x0400, got 0x%04X)\n", result);

    return 0;
}
