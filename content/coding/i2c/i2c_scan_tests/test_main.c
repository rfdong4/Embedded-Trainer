#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* Bus with 4 devices: accelerometer (0x1D), temp sensor (0x48), EEPROM (0x50), RTC (0x68) */
    uint8_t bus[] = {0x1D, 0x48, 0x50, 0x68, 0x00};

    /* TEST 1: Check for present device 0x50 (EEPROM) -> ACK (1) */
    uint8_t r1 = i2c_check_addr(bus, 0x50);
    if (r1 == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %u)\n", r1);

    /* TEST 2: Check for absent device 0x3C (OLED display) -> NACK (0) */
    uint8_t r2 = i2c_check_addr(bus, 0x3C);
    if (r2 == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0, got %u)\n", r2);

    /* TEST 3: Scan count with 4 devices */
    uint8_t r3 = i2c_scan_count(bus);
    if (r3 == 4) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 4, got %u)\n", r3);

    /* TEST 4: Scan count with empty bus (no devices) */
    uint8_t empty_bus[] = {0x00};
    uint8_t r4 = i2c_scan_count(empty_bus);
    if (r4 == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0, got %u)\n", r4);

    return 0;
}
