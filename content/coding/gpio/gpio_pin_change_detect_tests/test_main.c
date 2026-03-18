#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    uint16_t result;

    /* TEST 1: prev=0x0F, curr=0xF0 -> rising=0xF0, falling=0x0F -> 0x0FF0 */
    result = gpio_change_detect(0x0F, 0xF0);
    if (result == 0x0FF0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x0FF0, got 0x%04X)\n", result);

    /* TEST 2: No change: prev=0xAA, curr=0xAA -> 0x0000 */
    result = gpio_change_detect(0xAA, 0xAA);
    if (result == 0x0000) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x0000, got 0x%04X)\n", result);

    /* TEST 3: All pins rose: prev=0x00, curr=0xFF -> rising=0xFF, falling=0x00 -> 0x00FF */
    result = gpio_change_detect(0x00, 0xFF);
    if (result == 0x00FF) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0x00FF, got 0x%04X)\n", result);

    /* TEST 4: Mixed: prev=0b11001010, curr=0b10101100
       changed = XOR = 0b01100110
       rising  = ~prev & curr = 0b00100100 = 0x24
       falling = prev & ~curr = 0b01000010 = 0x42
       result  = 0x4224 */
    result = gpio_change_detect(0xCA, 0xAC);
    if (result == 0x4224) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0x4224, got 0x%04X)\n", result);

    return 0;
}
