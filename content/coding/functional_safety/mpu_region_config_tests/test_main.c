#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    MPU_Region region;
    uint8_t result;

    /* TEST 1: Valid region initialization (aligned base) */
    result = mpu_region_init(&region, 0x20000000, 0x1000, MPU_READ | MPU_WRITE);
    if (result == 1 && region.base_address == 0x20000000 && region.size == 0x1000
        && region.permissions == (MPU_READ | MPU_WRITE))
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (expected success with correct fields)\n");

    /* TEST 2: Invalid region initialization (unaligned base) */
    result = mpu_region_init(&region, 0x20000100, 0x1000, MPU_READ);
    if (result == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected failure for unaligned base, got %u)\n", result);

    /* TEST 3: Access check - read+write granted, execute denied */
    mpu_region_init(&region, 0x20000000, 0x1000, MPU_READ | MPU_WRITE);
    uint8_t can_read = mpu_check_access(&region, MPU_READ);
    uint8_t can_exec = mpu_check_access(&region, MPU_EXECUTE);
    if (can_read == 1 && can_exec == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected read=1 exec=0, got read=%u exec=%u)\n", can_read, can_exec);

    /* TEST 4: Address containment check */
    mpu_region_init(&region, 0x08000000, 0x10000, MPU_READ | MPU_EXECUTE);
    uint8_t inside = mpu_contains_address(&region, 0x08008000);
    uint8_t outside = mpu_contains_address(&region, 0x08010000);
    if (inside == 1 && outside == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected inside=1 outside=0, got inside=%u outside=%u)\n", inside, outside);

    return 0;
}
