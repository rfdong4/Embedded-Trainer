#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: M-mode (3) accessing mstatus (0x300) -- allowed */
    if (privilege_check(3, 0x300) == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1)\n");

    /* TEST 2: U-mode (0) accessing mstatus (0x300) -- denied */
    if (privilege_check(0, 0x300) == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0)\n");

    /* TEST 3: S-mode (1) accessing sstatus (0x100) -- allowed */
    if (privilege_check(1, 0x100) == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 1)\n");

    /* TEST 4: U-mode (0) accessing user CSR (0x000) -- allowed */
    if (privilege_check(0, 0x000) == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0)\n");

    return 0;
}
