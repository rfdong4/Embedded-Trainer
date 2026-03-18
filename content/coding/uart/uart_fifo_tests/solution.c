#include <stdint.h>

#define FIFO_SIZE 8

static uint8_t fifo_buf[FIFO_SIZE];
static int fifo_head;
static int fifo_tail;
static int fifo_cnt;

void uart_fifo_init(void) {
    fifo_head = 0;
    fifo_tail = 0;
    fifo_cnt = 0;
}

int uart_fifo_push(uint8_t data) {
    if (fifo_cnt >= FIFO_SIZE) return -1;
    fifo_buf[fifo_head] = data;
    fifo_head = (fifo_head + 1) % FIFO_SIZE;
    fifo_cnt++;
    return 0;
}

int uart_fifo_pop(void) {
    if (fifo_cnt == 0) return -1;
    uint8_t data = fifo_buf[fifo_tail];
    fifo_tail = (fifo_tail + 1) % FIFO_SIZE;
    fifo_cnt--;
    return data;
}

int uart_fifo_count(void) {
    return fifo_cnt;
}
