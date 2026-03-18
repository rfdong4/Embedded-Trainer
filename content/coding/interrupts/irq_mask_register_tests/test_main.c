#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Enable IRQ 5 on empty register */
    {
        uint32_t reg = enable_irq(0x00000000, 5);
        if (reg == 0x00000020) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 0x00000020, got 0x%08X)\n", reg);
    }

    /* TEST 2: Disable IRQ 3 from a register with bits 0-7 set */
    {
        uint32_t reg = disable_irq(0x000000FF, 3);
        if (reg == 0x000000F7) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 0x000000F7, got 0x%08X)\n", reg);
    }

    /* TEST 3: Check enabled and disabled IRQs */
    {
        uint32_t reg = 0x0000008A;  /* bits 1, 3, 7 set */
        uint8_t b1 = is_irq_enabled(reg, 3);
        uint8_t b2 = is_irq_enabled(reg, 4);
        if (b1 == 1 && b2 == 0) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 1,0 got %u,%u)\n", b1, b2);
    }

    /* TEST 4: Out-of-range irq_num (>= 32) is handled safely */
    {
        uint32_t reg1 = enable_irq(0x00000000, 32);
        uint32_t reg2 = disable_irq(0xFFFFFFFF, 33);
        uint8_t check = is_irq_enabled(0xFFFFFFFF, 32);
        if (reg1 == 0x00000000 && reg2 == 0xFFFFFFFF && check == 0)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (out-of-range handling failed)\n");
    }

    return 0;
}
