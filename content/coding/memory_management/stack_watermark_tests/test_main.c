#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Fully painted buffer has 0 usage */
    {
        uint32_t buf[16];
        stack_paint(buf, 16);
        uint32_t usage = stack_check_usage(buf, 16);
        if (usage == 0) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0, got %u)\n", usage);
    }

    /* TEST 2: Last 4 words overwritten -> usage = 4 */
    {
        uint32_t buf[16];
        stack_paint(buf, 16);
        buf[12] = 0x00000000;
        buf[13] = 0x11111111;
        buf[14] = 0x22222222;
        buf[15] = 0x33333333;
        uint32_t usage = stack_check_usage(buf, 16);
        if (usage == 4) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 4, got %u)\n", usage);
    }

    /* TEST 3: All words overwritten -> usage = size */
    {
        uint32_t buf[8];
        uint32_t i;
        stack_paint(buf, 8);
        for (i = 0; i < 8; i++) buf[i] = 0x00000000;
        uint32_t usage = stack_check_usage(buf, 8);
        if (usage == 8) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 8, got %u)\n", usage);
    }

    /* TEST 4: Only last word overwritten -> usage = 1 */
    {
        uint32_t buf[8];
        stack_paint(buf, 8);
        buf[7] = 0x12345678;
        uint32_t usage = stack_check_usage(buf, 8);
        if (usage == 1) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 1, got %u)\n", usage);
    }

    return 0;
}
