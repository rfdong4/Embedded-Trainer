#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Enable, disable, and query individual IRQs */
    {
        irq_mgr_init();
        irq_mgr_enable(0);
        irq_mgr_enable(5);
        irq_mgr_enable(31);
        int ok = (irq_mgr_is_enabled(0) == 1 &&
                  irq_mgr_is_enabled(5) == 1 &&
                  irq_mgr_is_enabled(31) == 1 &&
                  irq_mgr_is_enabled(1) == 0);
        irq_mgr_disable(5);
        ok = ok && (irq_mgr_is_enabled(5) == 0);
        /* Out of range should return 0 */
        ok = ok && (irq_mgr_is_enabled(32) == 0);
        if (ok)
            printf("TEST 1: PASS\n");
        else
            printf("TEST 1: FAIL (enable/disable/query mismatch)\n");
    }

    /* TEST 2: Save and restore mask correctly */
    {
        irq_mgr_init();
        irq_mgr_enable(3);
        irq_mgr_enable(7);
        int saved = irq_mgr_save();  /* save {3,7} */
        irq_mgr_disable(3);
        irq_mgr_enable(10);
        /* Current: {7, 10}. Saved: {3, 7} */
        int has10 = irq_mgr_is_enabled(10);
        int restored = irq_mgr_restore();
        int ok = (saved == 1 && restored == 1 &&
                  has10 == 1 &&
                  irq_mgr_is_enabled(3) == 1 &&
                  irq_mgr_is_enabled(7) == 1 &&
                  irq_mgr_is_enabled(10) == 0);
        if (ok)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (save/restore failed)\n");
    }

    /* TEST 3: Nested save/restore (two levels) */
    {
        irq_mgr_init();
        irq_mgr_enable(1);
        irq_mgr_save();          /* level 1: {1} */
        irq_mgr_enable(2);
        irq_mgr_save();          /* level 2: {1,2} */
        irq_mgr_disable(1);
        irq_mgr_disable(2);
        irq_mgr_enable(15);
        /* Current: {15} */
        irq_mgr_restore();       /* back to level 2: {1,2} */
        int ok1 = (irq_mgr_is_enabled(1) == 1 &&
                   irq_mgr_is_enabled(2) == 1 &&
                   irq_mgr_is_enabled(15) == 0);
        irq_mgr_restore();       /* back to level 1: {1} */
        int ok2 = (irq_mgr_is_enabled(1) == 1 &&
                   irq_mgr_is_enabled(2) == 0);
        if (ok1 && ok2)
            printf("TEST 3: PASS\n");
        else
            printf("TEST 3: FAIL (nested save/restore failed)\n");
    }

    /* TEST 4: Restore on empty stack returns 0, save overflow returns 0 */
    {
        irq_mgr_init();
        int empty_restore = irq_mgr_restore();  /* should return 0 */
        /* Fill the stack to capacity */
        int all_saved = 1;
        for (int i = 0; i < IRQ_SAVE_DEPTH; i++) {
            if (irq_mgr_save() != 1) { all_saved = 0; break; }
        }
        int overflow = irq_mgr_save();  /* should return 0 */
        if (empty_restore == 0 && all_saved == 1 && overflow == 0)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (expected restore=0 overflow=0, got %d %d)\n",
                   empty_restore, overflow);
    }

    return 0;
}
