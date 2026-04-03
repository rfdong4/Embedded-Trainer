#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint32_t result;

    /* TEST 1: Enable only => bit 0 = 1 => 0x0001 */
    result = dma_config_channel(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (result == 0x00000001) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x00000001, got 0x%08X)\n", result);

    /* TEST 2: P-to-M, circular, memory increment, TC interrupt, 16-bit periph,
       16-bit mem, high priority
       EN=1, TCIE=1, CIRC=1, MINC=1, psize=1, msize=1, priority=2
       bits: 0=1, 1=1, 5=1, 7=1, 8=1, 10=1, 12=2(bit13=1)
       = 0x01 | 0x02 | 0x20 | 0x80 | 0x100 | 0x400 | 0x2000 = 0x25A3 */
    result = dma_config_channel(1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 2);
    if (result == 0x000025A3) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x000025A3, got 0x%08X)\n", result);

    /* TEST 3: M-to-P, 32-bit both, very high priority, all interrupts
       EN=1, TCIE=1, HTIE=1, TEIE=1, DIR=1, PINC=0, MINC=1, psize=2, msize=2, pri=3
       = 0x01|0x02|0x04|0x08|0x10|0x80|0x200|0x800|0x3000 = 0x3A9F */
    result = dma_config_channel(1, 1, 1, 1, 1, 0, 0, 1, 2, 2, 3);
    if (result == 0x00003A9F) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x00003A9F, got 0x%08X)\n", result);

    /* TEST 4: All zeros */
    result = dma_config_channel(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (result == 0x00000000) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x00000000, got 0x%08X)\n", result);

    return 0;
}
