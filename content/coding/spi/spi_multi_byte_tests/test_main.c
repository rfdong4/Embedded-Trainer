#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "solution.c"

int main(void) {
    uint8_t rx[8];

    /* TEST 1: cmd=0xAA, data={0x55, 0xFF}, pattern=0x0F -> {0xA5, 0x5A, 0xF0} */
    uint8_t tx1[] = {0x55, 0xFF};
    memset(rx, 0, sizeof(rx));
    spi_multi_transfer(0xAA, tx1, rx, 2, 0x0F);
    if (rx[0] == 0xA5 && rx[1] == 0x5A && rx[2] == 0xF0)
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (expected {0xA5,0x5A,0xF0}, got {0x%02X,0x%02X,0x%02X})\n", rx[0], rx[1], rx[2]);

    /* TEST 2: cmd=0x9F, data={0x00, 0x00, 0x00}, pattern=0xFF -> {0x60, 0xFF, 0xFF, 0xFF} */
    uint8_t tx2[] = {0x00, 0x00, 0x00};
    memset(rx, 0, sizeof(rx));
    spi_multi_transfer(0x9F, tx2, rx, 3, 0xFF);
    if (rx[0] == 0x60 && rx[1] == 0xFF && rx[2] == 0xFF && rx[3] == 0xFF)
        printf("TEST 2: PASS\n");
    else
        printf("TEST 2: FAIL (expected {0x60,0xFF,0xFF,0xFF}, got {0x%02X,0x%02X,0x%02X,0x%02X})\n", rx[0], rx[1], rx[2], rx[3]);

    /* TEST 3: cmd=0x03, no data bytes, pattern=0x00 -> {0x03} */
    memset(rx, 0xAA, sizeof(rx));
    spi_multi_transfer(0x03, NULL, rx, 0, 0x00);
    if (rx[0] == 0x03)
        printf("TEST 3: PASS\n");
    else
        printf("TEST 3: FAIL (expected {0x03}, got {0x%02X})\n", rx[0]);

    /* TEST 4: cmd=0x00, data={0x12, 0x34, 0x56, 0x78}, pattern=0xAA -> {0xAA, 0xB8, 0x9E, 0xFC, 0xD2} */
    uint8_t tx4[] = {0x12, 0x34, 0x56, 0x78};
    memset(rx, 0, sizeof(rx));
    spi_multi_transfer(0x00, tx4, rx, 4, 0xAA);
    if (rx[0] == 0xAA && rx[1] == 0xB8 && rx[2] == 0x9E && rx[3] == 0xFC && rx[4] == 0xD2)
        printf("TEST 4: PASS\n");
    else
        printf("TEST 4: FAIL (expected {0xAA,0xB8,0x9E,0xFC,0xD2}, got {0x%02X,0x%02X,0x%02X,0x%02X,0x%02X})\n", rx[0], rx[1], rx[2], rx[3], rx[4]);

    return 0;
}
