#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: All 3 stages pass (all have bits 0 and 1 set) */
    uint8_t chain1[] = {0x03, 0x07, 0x03};
    int r1 = verify_boot_chain(chain1, 3);
    if (r1 == -1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected -1, got %d)\n", r1);

    /* TEST 2: Second stage fails (missing SIGNATURE_VALID bit) */
    uint8_t chain2[] = {0x03, 0x02, 0x03};
    int r2 = verify_boot_chain(chain2, 3);
    if (r2 == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 1, got %d)\n", r2);

    /* TEST 3: First stage fails (no flags set) */
    uint8_t chain3[] = {0x00, 0x03, 0x03};
    int r3 = verify_boot_chain(chain3, 3);
    if (r3 == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0, got %d)\n", r3);

    /* TEST 4: Last stage fails (VERSION_OK missing, only SIGNATURE_VALID) */
    uint8_t chain4[] = {0x03, 0x03, 0x03, 0x01};
    int r4 = verify_boot_chain(chain4, 4);
    if (r4 == 3) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 3, got %d)\n", r4);

    return 0;
}
