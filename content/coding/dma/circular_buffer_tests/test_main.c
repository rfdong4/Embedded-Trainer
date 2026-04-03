#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    circular_buffer_t cb;
    uint8_t backing[4];
    uint8_t data;
    int ret;

    /* TEST 1: Init and write/read single byte */
    cb_init(&cb, backing, 4);
    cb_write(&cb, 0xAA);
    ret = cb_read(&cb, &data);
    if (ret == 1 && data == 0xAA) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected ret=1 data=0xAA, got ret=%d data=0x%02X)\n", ret, data);

    /* TEST 2: Read from empty buffer returns 0 */
    ret = cb_read(&cb, &data);
    if (ret == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0 from empty buffer, got %d)\n", ret);

    /* TEST 3: Fill buffer completely, then write should fail */
    cb_init(&cb, backing, 4);
    cb_write(&cb, 0x01);
    cb_write(&cb, 0x02);
    cb_write(&cb, 0x03);
    cb_write(&cb, 0x04);
    ret = cb_write(&cb, 0x05);
    if (ret == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 0 from full buffer, got %d)\n", ret);

    /* TEST 4: Wrap-around: fill, drain half, refill, read all */
    cb_init(&cb, backing, 4);
    cb_write(&cb, 0x10);
    cb_write(&cb, 0x20);
    cb_write(&cb, 0x30);
    cb_write(&cb, 0x40);
    cb_read(&cb, &data); /* 0x10 */
    cb_read(&cb, &data); /* 0x20 */
    cb_write(&cb, 0x50);
    cb_write(&cb, 0x60);
    cb_read(&cb, &data); /* should be 0x30 */
    uint8_t d2, d3, d4;
    cb_read(&cb, &d2); /* 0x40 */
    cb_read(&cb, &d3); /* 0x50 */
    cb_read(&cb, &d4); /* 0x60 */
    if (data == 0x30 && d2 == 0x40 && d3 == 0x50 && d4 == 0x60) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 30,40,50,60 got %02X,%02X,%02X,%02X)\n", data, d2, d3, d4);

    return 0;
}
