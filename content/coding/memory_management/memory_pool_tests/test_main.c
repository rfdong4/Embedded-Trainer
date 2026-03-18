#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Allocate one block, should not be NULL */
    pool_init();
    {
        void *p = pool_alloc();
        if (p != NULL) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (expected non-NULL, got NULL)\n");
    }

    /* TEST 2: Allocate all 8 blocks, 9th should be NULL */
    pool_init();
    {
        void *ptrs[9];
        int i;
        int all_ok = 1;
        for (i = 0; i < 8; i++) {
            ptrs[i] = pool_alloc();
            if (ptrs[i] == NULL) { all_ok = 0; break; }
        }
        ptrs[8] = pool_alloc();
        if (all_ok && ptrs[8] == NULL) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (pool overflow not detected)\n");
    }

    /* TEST 3: Free a block and reallocate it */
    pool_init();
    {
        void *ptrs[8];
        int i;
        for (i = 0; i < 8; i++) {
            ptrs[i] = pool_alloc();
        }
        pool_free(ptrs[3]);
        void *realloc_ptr = pool_alloc();
        if (realloc_ptr == ptrs[3]) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected reuse of freed block)\n");
    }

    /* TEST 4: Free with invalid pointer does not corrupt pool */
    pool_init();
    {
        void *p1 = pool_alloc();
        uint8_t dummy;
        pool_free(&dummy);  /* invalid pointer - should be ignored */
        void *p2 = pool_alloc();
        /* p1 and p2 should be different since p1 was not freed */
        if (p1 != p2 && p2 != NULL) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (invalid free corrupted pool)\n");
    }

    return 0;
}
