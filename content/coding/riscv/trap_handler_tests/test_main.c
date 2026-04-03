#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Exception cause 2 (illegal instruction) */
    if (dispatch_trap(0x00000002, 15) == 2) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 2, got %d)\n", dispatch_trap(0x00000002, 15));

    /* TEST 2: Interrupt cause 7 (machine timer), bit 31 set -> 0x80000007 */
    if (dispatch_trap(0x80000007, 15) == 107) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 107, got %d)\n", dispatch_trap(0x80000007, 15));

    /* TEST 3: Invalid exception cause (20 > max_causes 15) */
    if (dispatch_trap(0x00000014, 15) == -1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected -1, got %d)\n", dispatch_trap(0x00000014, 15));

    /* TEST 4: Interrupt cause 11 (machine external) */
    if (dispatch_trap(0x8000000B, 15) == 111) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 111, got %d)\n", dispatch_trap(0x8000000B, 15));

    return 0;
}
