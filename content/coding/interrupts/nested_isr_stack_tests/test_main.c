#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Push first ISR, verify active priority and depth */
    {
        isr_stack_init();
        int idle_prio = isr_get_active_priority();
        int push_ok = isr_push(3);
        int ok = (idle_prio == 255 &&
                  push_ok == 1 &&
                  isr_get_active_priority() == 3 &&
                  isr_get_depth() == 1);
        if (ok)
            printf("TEST 1: PASS\n");
        else
            printf("TEST 1: FAIL (expected idle=255, push=1, active=3, depth=1)\n");
    }

    /* TEST 2: Higher priority preempts, equal/lower priority rejected */
    {
        isr_stack_init();
        isr_push(5);              /* active = 5, depth = 1 */
        int reject_eq = isr_push(5);   /* equal, should fail */
        int reject_lo = isr_push(10);  /* lower priority, should fail */
        int accept_hi = isr_push(2);   /* higher priority, should succeed */
        int ok = (reject_eq == 0 && reject_lo == 0 && accept_hi == 1 &&
                  isr_get_active_priority() == 2 && isr_get_depth() == 2);
        if (ok)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (expected reject equal/lower, accept higher)\n");
    }

    /* TEST 3: Pop restores previous context correctly */
    {
        isr_stack_init();
        isr_push(10);   /* depth 1 */
        isr_push(5);    /* depth 2 */
        isr_push(1);    /* depth 3 */
        int popped1 = isr_pop();   /* should return 1, active becomes 5 */
        uint8_t active_after1 = isr_get_active_priority();
        int depth_after1 = isr_get_depth();
        int popped2 = isr_pop();   /* should return 5, active becomes 10 */
        uint8_t active_after2 = isr_get_active_priority();
        int depth_after2 = isr_get_depth();
        int ok = (popped1 == 1 && active_after1 == 5 && depth_after1 == 2 &&
                  popped2 == 5 && active_after2 == 10 && depth_after2 == 1);
        if (ok)
            printf("TEST 3: PASS\n");
        else
            printf("TEST 3: FAIL (pop did not restore context correctly)\n");
    }

    /* TEST 4: Pop on empty returns -1; stack overflow rejected */
    {
        isr_stack_init();
        int empty_pop = isr_pop();  /* should return -1 */
        /* Fill stack to max depth */
        int all_pushed = 1;
        for (int i = ISR_MAX_DEPTH; i >= 1; i--) {
            if (isr_push((uint8_t)i) != 1) { all_pushed = 0; break; }
        }
        /* Try to push priority 0 (highest) - should fail, stack full */
        int overflow = isr_push(0);
        int ok = (empty_pop == -1 && all_pushed == 1 && overflow == 0 &&
                  isr_get_depth() == ISR_MAX_DEPTH);
        if (ok)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (expected pop=-1, overflow=0, depth=%d)\n",
                   isr_get_depth());
    }

    return 0;
}
