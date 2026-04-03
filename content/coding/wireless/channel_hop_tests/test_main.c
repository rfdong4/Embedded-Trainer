#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    /* TEST 1: All channels usable, simple hop
       current=0, hop=5, all 8 channels enabled, num=8
       next = (0+5)%8 = 5, channel 5 is enabled -> 5 */
    uint8_t r1 = next_channel(0, 5, 0xFF, 8);
    if (r1 == 5) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected 5, got %d)\n", r1);

    /* TEST 2: Wrap around
       current=6, hop=5, all 8 channels enabled, num=8
       next = (6+5)%8 = 3 -> 3 */
    uint8_t r2 = next_channel(6, 5, 0xFF, 8);
    if (r2 == 3) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected 3, got %d)\n", r2);

    /* TEST 3: Skip disabled channel
       current=0, hop=2, channel_map=0b11111011 (ch 2 disabled), num=8
       first try: (0+2)%8 = 2, ch 2 disabled
       next try: (2+2)%8 = 4, ch 4 enabled -> 4 */
    uint8_t r3 = next_channel(0, 2, 0xFB, 8);
    if (r3 == 4) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected 4, got %d)\n", r3);

    /* TEST 4: Only one channel enabled (channel 3)
       current=0, hop=1, channel_map=0x08, num=8
       keeps hopping until landing on channel 3 -> 3 */
    uint8_t r4 = next_channel(0, 1, 0x08, 8);
    if (r4 == 3) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected 3, got %d)\n", r4);

    return 0;
}
