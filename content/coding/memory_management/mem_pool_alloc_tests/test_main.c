#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Init gives 16 free blocks, first alloc succeeds */
    pool_init();
    {
        int free_before = pool_count_free();
        void *p = pool_alloc();
        int free_after = pool_count_free();
        if (free_before == 16 && p != NULL && free_after == 15)
            printf("TEST 1: PASS\n");
        else
            printf("TEST 1: FAIL (expected 16 free then 15, got %d then %d)\n",
                   free_before, free_after);
    }

    /* TEST 2: Allocate all 16, then 17th returns NULL */
    pool_init();
    {
        void *ptrs[16];
        int all_ok = 1;
        for (int i = 0; i < 16; i++) {
            ptrs[i] = pool_alloc();
            if (ptrs[i] == NULL) { all_ok = 0; break; }
        }
        void *overflow = pool_alloc();
        int free_count = pool_count_free();
        if (all_ok && overflow == NULL && free_count == 0)
            printf("TEST 2: PASS\n");
        else
            printf("TEST 2: FAIL (expected all alloc ok and NULL overflow, free=%d)\n",
                   free_count);
        (void)ptrs;
    }

    /* TEST 3: Free a block and reallocate it (lowest-numbered reuse) */
    pool_init();
    {
        void *ptrs[16];
        for (int i = 0; i < 16; i++)
            ptrs[i] = pool_alloc();
        pool_free(ptrs[5]);
        pool_free(ptrs[10]);
        void *realloc_ptr = pool_alloc();
        /* Should get block 5 back (lowest free) */
        if (realloc_ptr == ptrs[5] && pool_count_free() == 1)
            printf("TEST 3: PASS\n");
        else
            printf("TEST 3: FAIL (expected reuse of block 5)\n");
    }

    /* TEST 4: Free with invalid pointer does not corrupt pool */
    pool_init();
    {
        void *p1 = pool_alloc();
        void *p2 = pool_alloc();
        uint8_t dummy;
        pool_free(&dummy);  /* invalid pointer */
        int free_count = pool_count_free();
        if (free_count == 14 && p1 != NULL && p2 != NULL)
            printf("TEST 4: PASS\n");
        else
            printf("TEST 4: FAIL (expected 14 free, got %d)\n", free_count);
    }

    return 0;
}
