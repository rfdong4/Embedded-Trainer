#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "solution.c"

int main(void) {
    /* Simulated device register map (e.g., a temperature sensor) */
    uint8_t reg_map[] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80};
    uint8_t out[8];
    int8_t ret;

    /* TEST 1: Read 2 bytes starting at register 1 -> {0x20, 0x30} */
    memset(out, 0, sizeof(out));
    ret = i2c_reg_read(reg_map, 8, 1, out, 2);
    if (ret == 2 && out[0] == 0x20 && out[1] == 0x30)
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (expected ret=2 out={0x20,0x30}, got ret=%d out={0x%02X,0x%02X})\n", ret, out[0], out[1]);

    /* TEST 2: Read all 8 bytes from start -> {0x10..0x80} */
    memset(out, 0, sizeof(out));
    ret = i2c_reg_read(reg_map, 8, 0, out, 8);
    if (ret == 8 && out[0] == 0x10 && out[7] == 0x80)
        printf("TEST 2: PASS\n");
    else
        printf("TEST 2: FAIL (expected ret=8 out[0]=0x10 out[7]=0x80, got ret=%d out[0]=0x%02X out[7]=0x%02X)\n", ret, out[0], out[7]);

    /* TEST 3: Out-of-bounds read (reg_addr=6, len=3 exceeds map_size=8) -> -1 */
    ret = i2c_reg_read(reg_map, 8, 6, out, 3);
    if (ret == -1)
        printf("TEST 3: PASS\n");
    else
        printf("TEST 3: FAIL (expected -1, got %d)\n", ret);

    /* TEST 4: Invalid start address (reg_addr=10, map_size=8) -> -1 */
    ret = i2c_reg_read(reg_map, 8, 10, out, 1);
    if (ret == -1)
        printf("TEST 4: PASS\n");
    else
        printf("TEST 4: FAIL (expected -1, got %d)\n", ret);

    return 0;
}
