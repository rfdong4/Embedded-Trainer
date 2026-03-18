#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Initial state - depth 0, active -1 */
    irq_tracker_init();
    {
        int depth = irq_get_depth();
        int active = irq_get_active();
        if (depth == 0 && active == -1) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected depth=0 active=-1, got depth=%d active=%d)\n", depth, active);
    }

    /* TEST 2: Enter and exit nested interrupts */
    irq_tracker_init();
    {
        irq_enter(5);
        irq_enter(2);
        int d1 = irq_get_depth();
        int a1 = irq_get_active();
        irq_exit();
        int d2 = irq_get_depth();
        int a2 = irq_get_active();
        irq_exit();
        int d3 = irq_get_depth();
        int a3 = irq_get_active();
        if (d1 == 2 && a1 == 2 && d2 == 1 && a2 == 5 && d3 == 0 && a3 == -1)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (nested enter/exit incorrect)\n");
    }

    /* TEST 3: Overflow protection - max depth 8 */
    irq_tracker_init();
    {
        int i;
        for (i = 0; i < 10; i++) irq_enter((uint8_t)i);
        int depth = irq_get_depth();
        int active = irq_get_active();
        /* Only 8 should fit; active should be IRQ 7 (last successful push) */
        if (depth == 8 && active == 7) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected depth=8 active=7, got depth=%d active=%d)\n", depth, active);
    }

    /* TEST 4: Exit on empty does nothing */
    irq_tracker_init();
    {
        irq_exit();  /* should be no-op */
        irq_exit();  /* should be no-op */
        int depth = irq_get_depth();
        int active = irq_get_active();
        irq_enter(10);
        int d2 = irq_get_depth();
        int a2 = irq_get_active();
        if (depth == 0 && active == -1 && d2 == 1 && a2 == 10)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (exit on empty corrupted state)\n");
    }

    return 0;
}
