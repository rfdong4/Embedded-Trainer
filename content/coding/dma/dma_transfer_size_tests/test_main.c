#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t count;
    uint8_t aligned;

    /* TEST 1: 1024 bytes with word (4-byte) width = 256 transfers */
    count = calc_transfer_count(1024, 4);
    if (count == 256) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 256, got %u)\n", count);

    /* TEST 2: 10 bytes with word width = 3 transfers (rounds up) */
    count = calc_transfer_count(10, 4);
    if (count == 3) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 3, got %u)\n", count);

    /* TEST 3: Address 0x20000004 is word-aligned, 0x20000003 is not */
    aligned = check_alignment(0x20000004, 4);
    uint8_t not_aligned = check_alignment(0x20000003, 4);
    if (aligned == 1 && not_aligned == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (aligned=%u not_aligned=%u)\n", aligned, not_aligned);

    /* TEST 4: Halfword alignment: 0x20000002 ok, 0x20000001 not */
    aligned = check_alignment(0x20000002, 2);
    not_aligned = check_alignment(0x20000001, 2);
    uint32_t count2 = calc_transfer_count(7, 2);  /* 4 halfword transfers */
    if (aligned == 1 && not_aligned == 0 && count2 == 4) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (a=%u na=%u cnt=%u)\n", aligned, not_aligned, count2);

    return 0;
}
