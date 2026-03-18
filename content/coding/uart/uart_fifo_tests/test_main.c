#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    int result;
    int pass;

    /* TEST 1: Push and pop single byte */
    uart_fifo_init();
    uart_fifo_push(0xAB);
    result = uart_fifo_pop();
    if (result == 0xAB && uart_fifo_count() == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected pop=0xAB count=0, got pop=0x%02X count=%d)\n", result, uart_fifo_count());

    /* TEST 2: FIFO ordering (first in, first out) */
    uart_fifo_init();
    uart_fifo_push(0x01);
    uart_fifo_push(0x02);
    uart_fifo_push(0x03);
    pass = 1;
    if (uart_fifo_count() != 3) pass = 0;
    if (uart_fifo_pop() != 0x01) pass = 0;
    if (uart_fifo_pop() != 0x02) pass = 0;
    if (uart_fifo_pop() != 0x03) pass = 0;
    if (pass) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (FIFO order incorrect)\n");

    /* TEST 3: Full FIFO returns -1 on push */
    uart_fifo_init();
    for (int i = 0; i < 8; i++) uart_fifo_push((uint8_t)i);
    result = uart_fifo_push(0xFF);
    if (result == -1 && uart_fifo_count() == 8) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected push=-1 count=8, got push=%d count=%d)\n", result, uart_fifo_count());

    /* TEST 4: Empty FIFO returns -1 on pop */
    uart_fifo_init();
    result = uart_fifo_pop();
    if (result == -1 && uart_fifo_count() == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected pop=-1 count=0, got pop=%d count=%d)\n", result, uart_fifo_count());

    return 0;
}
