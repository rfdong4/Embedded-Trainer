#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* Priority table: priorities[irq] = priority_value */
    /* IRQ: 0  1  2  3  4  5  6  7 */
    uint8_t prio[8] = {3, 1, 4, 1, 5, 9, 2, 6};

    /* TEST 1: Single pending IRQ */
    {
        uint8_t pending[] = {4};
        uint8_t result = next_irq_to_service(pending, 1, prio);
        if (result == 4) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected 4, got %u)\n", result);
    }

    /* TEST 2: Multiple pending, clear winner (IRQ 6 has priority 2) */
    {
        uint8_t pending[] = {0, 4, 6, 5};
        uint8_t result = next_irq_to_service(pending, 4, prio);
        /* prio[0]=3, prio[4]=5, prio[6]=2, prio[5]=9 -> IRQ 6 wins */
        if (result == 6) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (expected 6, got %u)\n", result);
    }

    /* TEST 3: Tie-breaking by IRQ number (IRQ 1 and 3 both have priority 1) */
    {
        uint8_t pending[] = {3, 1, 5};
        uint8_t result = next_irq_to_service(pending, 3, prio);
        /* prio[3]=1, prio[1]=1, prio[5]=9 -> tie at 1, IRQ 1 < IRQ 3 */
        if (result == 1) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected 1, got %u)\n", result);
    }

    /* TEST 4: Empty pending list returns 255 */
    {
        uint8_t result = next_irq_to_service(NULL, 0, prio);
        if (result == 255) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected 255, got %u)\n", result);
    }

    return 0;
}
