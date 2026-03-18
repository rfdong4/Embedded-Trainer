#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "solution.c"

int main(void) {
    /* Source data: bytes 0x00 through 0x1F */
    uint8_t src[32];
    for (int i = 0; i < 32; i++) src[i] = (uint8_t)i;

    /* TEST 1: Single descriptor copy */
    {
        uint8_t dst[32] = {0};
        SG_Descriptor desc[] = {{4, 3}};  /* copy bytes 4,5,6 */
        uint32_t copied = sg_copy(src, dst, desc, 1);
        if (copied == 3 && dst[0] == 4 && dst[1] == 5 && dst[2] == 6)
            printf("TEST 1: PASS\n");
        else
            printf("TEST 1: FAIL (expected 3 bytes [4,5,6], got %u bytes [%u,%u,%u])\n",
                   copied, dst[0], dst[1], dst[2]);
    }

    /* TEST 2: Multiple descriptors gather non-contiguous regions */
    {
        uint8_t dst[32] = {0};
        SG_Descriptor desc[] = {{0, 2}, {10, 3}, {20, 4}};
        /* Should copy: src[0..1], src[10..12], src[20..23] */
        uint32_t copied = sg_copy(src, dst, desc, 3);
        int ok = (copied == 9 &&
                  dst[0] == 0 && dst[1] == 1 &&
                  dst[2] == 10 && dst[3] == 11 && dst[4] == 12 &&
                  dst[5] == 20 && dst[6] == 21 && dst[7] == 22 && dst[8] == 23);
        if (ok)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (expected 9 bytes gathered, got %u)\n", copied);
    }

    /* TEST 3: sg_total_length calculates correct sum */
    {
        SG_Descriptor desc[] = {{0, 5}, {10, 0}, {15, 8}, {25, 3}};
        uint32_t total = sg_total_length(desc, 4);
        if (total == 16)
            printf("TEST 3: PASS\n");
        else
            printf("TEST 3: FAIL (expected total 16, got %u)\n", total);
    }

    /* TEST 4: Zero-length descriptors are skipped, order preserved */
    {
        uint8_t dst[32] = {0};
        SG_Descriptor desc[] = {{5, 2}, {0, 0}, {15, 1}};
        uint32_t copied = sg_copy(src, dst, desc, 3);
        if (copied == 3 && dst[0] == 5 && dst[1] == 6 && dst[2] == 15)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (expected 3 bytes [5,6,15], got %u bytes [%u,%u,%u])\n",
                   copied, dst[0], dst[1], dst[2]);
    }

    return 0;
}
