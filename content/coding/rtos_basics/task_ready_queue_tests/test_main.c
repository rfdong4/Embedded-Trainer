#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Set tasks 2 and 5 ready */
    uint8_t mask = 0;
    mask = set_task_ready(mask, 2);
    mask = set_task_ready(mask, 5);
    if (mask == 0x24) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 0x24, got 0x%02X)\n", mask);

    /* TEST 2: Clear task 2, mask should be 0x20 */
    mask = clear_task_ready(mask, 2);
    if (mask == 0x20) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0x20, got 0x%02X)\n", mask);

    /* TEST 3: Get highest priority from mask with tasks 3,5,7 ready */
    mask = 0xA8;  /* bits 3,5,7 = 0b10101000 */
    int8_t highest = get_highest_ready(mask);
    if (highest == 3) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 3, got %d)\n", highest);

    /* TEST 4: Get highest priority from empty mask returns -1 */
    highest = get_highest_ready(0x00);
    if (highest == -1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected -1, got %d)\n", highest);

    return 0;
}
