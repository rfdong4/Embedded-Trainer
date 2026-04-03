#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    dma_descriptor_t desc;

    /* TEST 1: Byte transfer, no increment, no next */
    build_descriptor(&desc, 0x40004800, 0x20001000, 128, 0, 0, 0, 0);
    if (desc.src_addr == 0x40004800 && desc.dst_addr == 0x20001000 &&
        desc.count == 128 && desc.control == 0x01 && desc.next == 0)
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (ctrl=0x%08X)\n", desc.control);

    /* TEST 2: Word transfer, both increment, with next descriptor */
    build_descriptor(&desc, 0x20000000, 0x20002000, 256, 2, 1, 1, 0x20003000);
    /* control: enable=1, width=2<<1=4, src_inc=1<<3=8, dst_inc=1<<4=16 => 0x1D */
    if (desc.src_addr == 0x20000000 && desc.dst_addr == 0x20002000 &&
        desc.count == 256 && desc.control == 0x1D && desc.next == 0x20003000)
        printf("TEST 2: PASS\n");
    else
        printf("TEST 2: FAIL (ctrl=0x%08X next=0x%08X)\n", desc.control, desc.next);

    /* TEST 3: Halfword transfer, source increment only */
    build_descriptor(&desc, 0x20000100, 0x40004C00, 64, 1, 1, 0, 0);
    /* control: enable=1, width=1<<1=2, src_inc=1<<3=8 => 0x0B */
    if (desc.control == 0x0B)
        printf("TEST 3: PASS\n");
    else
        printf("TEST 3: FAIL (expected 0x0B, got 0x%08X)\n", desc.control);

    /* TEST 4: Word transfer, dest increment only */
    build_descriptor(&desc, 0x40004800, 0x20000000, 512, 2, 0, 1, 0);
    /* control: enable=1, width=2<<1=4, dst_inc=1<<4=16 => 0x15 */
    if (desc.control == 0x15 && desc.count == 512)
        printf("TEST 4: PASS\n");
    else
        printf("TEST 4: FAIL (expected 0x15, got 0x%08X)\n", desc.control);

    return 0;
}
