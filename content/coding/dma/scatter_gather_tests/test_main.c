#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "solution.c"

int main(void) {
    dma_descriptor_t descs[4];
    uint32_t ret;

    /* TEST 1: Single descriptor - next should be 0 */
    descs[0].valid = 0;
    descs[0].next = 0xDEAD;
    ret = build_sg_list(descs, 1);
    if (ret == 1 && descs[0].next == 0 && descs[0].valid == 1)
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (ret=%u next=0x%08X valid=%u)\n", ret, descs[0].next, descs[0].valid);

    /* TEST 2: Two descriptors - first links to second, second is terminal */
    descs[0].valid = 0;
    descs[1].valid = 0;
    ret = build_sg_list(descs, 2);
    uint32_t expected_next = (uint32_t)(uintptr_t)&descs[1];
    if (ret == 2 && descs[0].next == expected_next && descs[1].next == 0)
        printf("TEST 2: PASS\n");
    else
        printf("TEST 2: FAIL\n");

    /* TEST 3: Four descriptors - all valid */
    ret = build_sg_list(descs, 4);
    int all_valid = descs[0].valid && descs[1].valid && descs[2].valid && descs[3].valid;
    if (ret == 4 && all_valid)
        printf("TEST 3: PASS\n");
    else
        printf("TEST 3: FAIL (ret=%u valid=%d)\n", ret, all_valid);

    /* TEST 4: Zero count returns 0 */
    ret = build_sg_list(descs, 0);
    if (ret == 0)
        printf("TEST 4: PASS\n");
    else
        printf("TEST 4: FAIL (expected 0, got %u)\n", ret);

    return 0;
}
