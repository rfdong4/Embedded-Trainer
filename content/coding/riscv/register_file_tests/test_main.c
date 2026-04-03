#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "solution.c"

int main(void) {
    uint32_t regs[32];
    memset(regs, 0, sizeof(regs));

    /* TEST 1: x0 always reads as zero */
    regs[0] = 0xDEADBEEF;
    if (reg_read(regs, 0) == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0, got 0x%08X)\n", reg_read(regs, 0));

    /* TEST 2: Write and read x10 (a0) */
    reg_write(regs, 10, 0x12345678);
    if (reg_read(regs, 10) == 0x12345678) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x12345678, got 0x%08X)\n", reg_read(regs, 10));

    /* TEST 3: Write to x0 is ignored */
    reg_write(regs, 0, 0xFFFFFFFF);
    if (reg_read(regs, 0) == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got 0x%08X)\n", reg_read(regs, 0));

    /* TEST 4: Write and read x31 */
    reg_write(regs, 31, 0xCAFEBABE);
    if (reg_read(regs, 31) == 0xCAFEBABE) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0xCAFEBABE, got 0x%08X)\n", reg_read(regs, 31));

    return 0;
}
