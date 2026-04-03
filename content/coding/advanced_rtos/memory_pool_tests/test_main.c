#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "solution.c"

int main(void) {
    uint8_t buffer[128];
    MemPool pool;

    /* TEST 1: Init and first alloc returns buffer start */
    pool_init(&pool, buffer, 32, 4);
    void *b1 = pool_alloc(&pool);
    if (b1 == buffer) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected %p, got %p)\n", (void*)buffer, b1);

    /* TEST 2: Second alloc returns next block */
    void *b2 = pool_alloc(&pool);
    if (b2 == buffer + 32) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected %p, got %p)\n", (void*)(buffer + 32), b2);

    /* TEST 3: Alloc all blocks, then alloc returns NULL */
    {
        pool_alloc(&pool);  /* b3 */
        pool_alloc(&pool);  /* b4 */
        void *b5 = pool_alloc(&pool);  /* should be NULL */
        if (b5 == NULL) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (expected NULL, got %p)\n", b5);
    }

    /* TEST 4: Free and re-alloc returns freed block */
    {
        pool_free(&pool, b1);
        void *b6 = pool_alloc(&pool);
        if (b6 == b1) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (expected %p, got %p)\n", b1, b6);
    }

    return 0;
}
