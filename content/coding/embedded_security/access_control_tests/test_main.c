#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: User has READ+WRITE (0x03), requires READ (0x01) -> granted */
    int r1 = check_access(0x03, 0x01);
    if (r1 == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 1, got %d)\n", r1);

    /* TEST 2: User has READ (0x01), requires READ+WRITE (0x03) -> denied */
    int r2 = check_access(0x01, 0x03);
    if (r2 == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 0, got %d)\n", r2);

    /* TEST 3: User has all perms (0x0F), requires ADMIN+EXECUTE (0x0C) -> granted */
    int r3 = check_access(0x0F, 0x0C);
    if (r3 == 1) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 1, got %d)\n", r3);

    /* TEST 4: User has no perms (0x00), requires READ (0x01) -> denied */
    int r4 = check_access(0x00, 0x01);
    if (r4 == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 0, got %d)\n", r4);

    return 0;
}
