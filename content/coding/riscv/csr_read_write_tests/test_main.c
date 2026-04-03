#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t csr;
    uint32_t old;

    /* TEST 1: Read a CSR with known value */
    csr = 0x00000080;
    if (csr_read(&csr) == 0x00000080) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00000080, got 0x%08X)\n", csr_read(&csr));

    /* TEST 2: Set bits and verify old value returned */
    csr = 0x00000008;
    old = csr_set_bits(&csr, 0x00000003);
    if (old == 0x00000008 && csr == 0x0000000B) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (old=0x%08X, csr=0x%08X)\n", old, csr);

    /* TEST 3: Clear bits and verify old value returned */
    csr = 0x000000FF;
    old = csr_clear_bits(&csr, 0x0000000F);
    if (old == 0x000000FF && csr == 0x000000F0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (old=0x%08X, csr=0x%08X)\n", old, csr);

    /* TEST 4: Set bits on zero CSR */
    csr = 0x00000000;
    old = csr_set_bits(&csr, 0x80000000);
    if (old == 0x00000000 && csr == 0x80000000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (old=0x%08X, csr=0x%08X)\n", old, csr);

    return 0;
}
