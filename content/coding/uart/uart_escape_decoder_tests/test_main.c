#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint8_t output[32];
    int len;

    /* TEST 1: No escapes - passthrough */
    {
        uint8_t input[] = {0x41, 0x42, 0x43, 0x44};
        len = uart_escape_decode(input, 4, output);
        if (len == 4 && output[0] == 0x41 && output[1] == 0x42 &&
            output[2] == 0x43 && output[3] == 0x44)
            printf("TEST 1: PASS\n");
        else
            printf("TEST 1: FAIL (expected len=4 data=41424344, got len=%d)\n", len);
    }

    /* TEST 2: Single escape - 0x7D 0x5E decodes to 0x7E */
    {
        uint8_t input[] = {0x41, 0x7D, 0x5E, 0x42};
        len = uart_escape_decode(input, 4, output);
        if (len == 3 && output[0] == 0x41 && output[1] == 0x7E && output[2] == 0x42)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (expected len=3 data=41 7E 42, got len=%d data=%02X %02X %02X)\n",
                   len, output[0], output[1], output[2]);
    }

    /* TEST 3: Escaped escape char - 0x7D 0x5D decodes to 0x7D */
    {
        uint8_t input[] = {0x10, 0x7D, 0x5D, 0x20, 0x7D, 0x5D};
        len = uart_escape_decode(input, 6, output);
        if (len == 4 && output[0] == 0x10 && output[1] == 0x7D &&
            output[2] == 0x20 && output[3] == 0x7D)
            printf("TEST 3: PASS\n");
        else
            printf("TEST 3: FAIL (expected len=4 data=10 7D 20 7D, got len=%d)\n", len);
    }

    /* TEST 4: Multiple consecutive escapes */
    {
        uint8_t input[] = {0x7D, 0x5E, 0x7D, 0x5D, 0x7D, 0x5E};
        len = uart_escape_decode(input, 6, output);
        if (len == 3 && output[0] == 0x7E && output[1] == 0x7D && output[2] == 0x7E)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (expected len=3 data=7E 7D 7E, got len=%d)\n", len);
    }

    return 0;
}
