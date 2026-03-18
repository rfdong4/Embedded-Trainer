#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Non-overlapping copy */
    {
        uint8_t src[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        uint8_t dst[8] = {0};
        safe_memcpy(dst, src, 8);
        if (memcmp(dst, src, 8) == 0) printf("TEST 1: PASS\n");
        else printf("TEST 1: FAIL (non-overlapping copy incorrect)\n");
    }

    /* TEST 2: Overlapping copy forward (dst > src) */
    {
        uint8_t buf[10] = {1, 2, 3, 4, 5, 6, 7, 8, 0, 0};
        /* Copy buf[0..4] to buf[2..6]: src=buf, dst=buf+2, n=5 */
        safe_memcpy(buf + 2, buf, 5);
        uint8_t expected[10] = {1, 2, 1, 2, 3, 4, 5, 8, 0, 0};
        if (memcmp(buf, expected, 10) == 0) printf("TEST 2: PASS\n");
        else printf("TEST 2: FAIL (overlap dst>src copy incorrect)\n");
    }

    /* TEST 3: Overlapping copy backward (dst < src) */
    {
        uint8_t buf[10] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8};
        /* Copy buf[2..6] to buf[0..4]: src=buf+2, dst=buf, n=5 */
        safe_memcpy(buf, buf + 2, 5);
        uint8_t expected[10] = {1, 2, 3, 4, 5, 4, 5, 6, 7, 8};
        if (memcmp(buf, expected, 10) == 0) printf("TEST 3: PASS\n");
        else printf("TEST 3: FAIL (overlap dst<src copy incorrect)\n");
    }

    /* TEST 4: Zero-length copy (no-op) and return value */
    {
        uint8_t buf[4] = {0xAA, 0xBB, 0xCC, 0xDD};
        void *ret = safe_memcpy(buf, buf + 2, 0);
        uint8_t expected[4] = {0xAA, 0xBB, 0xCC, 0xDD};
        if (ret == buf && memcmp(buf, expected, 4) == 0) printf("TEST 4: PASS\n");
        else printf("TEST 4: FAIL (zero-length copy failed)\n");
    }

    return 0;
}
