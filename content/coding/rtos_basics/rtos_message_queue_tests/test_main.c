#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    MsgQueue q;
    uint32_t msg;

    /* TEST 1: Send and receive a single message */
    mq_init(&q);
    int s = mq_send(&q, 0xDEADBEEF);
    int r = mq_receive(&q, &msg);
    if (s == 1 && r == 1 && msg == 0xDEADBEEF && mq_is_empty(&q)) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected s=1 r=1 msg=0xDEADBEEF empty=1, got s=%d r=%d msg=0x%08X empty=%d)\n", s, r, msg, mq_is_empty(&q));

    /* TEST 2: Fill the queue to capacity, verify full */
    mq_init(&q);
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) mq_send(&q, (uint32_t)(i + 1));
    int full = mq_is_full(&q);
    int overflow = mq_send(&q, 0xFF);
    if (full == 1 && overflow == 0 && mq_count(&q) == MAX_QUEUE_SIZE) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected full=1 overflow=0 count=%d, got full=%d overflow=%d count=%u)\n", MAX_QUEUE_SIZE, full, overflow, mq_count(&q));

    /* TEST 3: FIFO ordering preserved */
    mq_init(&q);
    mq_send(&q, 10);
    mq_send(&q, 20);
    mq_send(&q, 30);
    uint32_t a, b, c;
    mq_receive(&q, &a);
    mq_receive(&q, &b);
    mq_receive(&q, &c);
    if (a == 10 && b == 20 && c == 30) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 10,20,30, got %u,%u,%u)\n", a, b, c);

    /* TEST 4: Wrap-around behavior */
    mq_init(&q);
    for (int i = 0; i < 6; i++) mq_send(&q, (uint32_t)(i + 100));
    for (int i = 0; i < 4; i++) mq_receive(&q, &msg);
    for (int i = 0; i < 6; i++) mq_send(&q, (uint32_t)(i + 200));
    uint8_t cnt = mq_count(&q);
    mq_receive(&q, &msg);
    if (cnt == 8 && msg == 104) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected count=8 first_msg=104, got count=%u first_msg=%u)\n", cnt, msg);

    return 0;
}
