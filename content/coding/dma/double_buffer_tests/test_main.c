#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    double_buffer_t db;

    /* TEST 1: After init, no buffer is ready */
    db_init(&db);
    int idx = db_get_ready_buffer(&db);
    if (idx == -1 && db.active_buffer == 0)
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (idx=%d active=%u)\n", idx, db.active_buffer);

    /* TEST 2: Swap makes buffer 0 ready, active becomes 1 */
    db_init(&db);
    uint8_t completed = db_swap(&db);
    if (completed == 0 && db.active_buffer == 1 && db.buffer_ready[0] == 1)
        printf("TEST 2: PASS\n");
    else
        printf("TEST 2: FAIL (completed=%u active=%u ready0=%u)\n",
               completed, db.active_buffer, db.buffer_ready[0]);

    /* TEST 3: get_ready_buffer returns 0 after first swap, then -1 */
    db_init(&db);
    db_swap(&db);
    idx = db_get_ready_buffer(&db);
    int idx2 = db_get_ready_buffer(&db);
    if (idx == 0 && idx2 == -1)
        printf("TEST 3: PASS\n");
    else
        printf("TEST 3: FAIL (idx=%d idx2=%d)\n", idx, idx2);

    /* TEST 4: Two swaps without processing - both buffers ready */
    db_init(&db);
    db_swap(&db);  /* buffer 0 ready, active=1 */
    db_swap(&db);  /* buffer 1 ready, active=0 */
    int r0 = db_get_ready_buffer(&db);  /* should get 0 first */
    int r1 = db_get_ready_buffer(&db);  /* should get 1 */
    int r2 = db_get_ready_buffer(&db);  /* should get -1 */
    if (r0 == 0 && r1 == 1 && r2 == -1)
        printf("TEST 4: PASS\n");
    else
        printf("TEST 4: FAIL (r0=%d r1=%d r2=%d)\n", r0, r1, r2);

    return 0;
}
