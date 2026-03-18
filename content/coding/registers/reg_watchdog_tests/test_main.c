#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    Watchdog w;

    /* TEST 1: Configure and count down without expiring */
    wdt_init(&w);
    int cfg = wdt_configure(&w, 5);
    int e1 = wdt_check_expired(&w);
    int e2 = wdt_check_expired(&w);
    uint32_t cnt = wdt_get_counter(&w);
    if (cfg == 1 && e1 == 0 && e2 == 0 && cnt == 3) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected cfg=1 e1=0 e2=0 cnt=3, got cfg=%d e1=%d e2=%d cnt=%u)\n", cfg, e1, e2, cnt);

    /* TEST 2: Count down to expiration */
    wdt_init(&w);
    wdt_configure(&w, 3);
    wdt_check_expired(&w);
    wdt_check_expired(&w);
    int expired = wdt_check_expired(&w);
    if (expired == 1 && wdt_get_counter(&w) == 0) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected expired=1 cnt=0, got expired=%d cnt=%u)\n", expired, wdt_get_counter(&w));

    /* TEST 3: Kick resets the counter */
    wdt_init(&w);
    wdt_configure(&w, 4);
    wdt_check_expired(&w);
    wdt_check_expired(&w);
    int k = wdt_kick(&w);
    uint32_t after_kick = wdt_get_counter(&w);
    if (k == 1 && after_kick == 4) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected k=1 cnt=4, got k=%d cnt=%u)\n", k, after_kick);

    /* TEST 4: Disable stops counting, configure with 0 fails */
    wdt_init(&w);
    wdt_configure(&w, 3);
    wdt_disable(&w);
    int e_dis = wdt_check_expired(&w);
    int k_dis = wdt_kick(&w);
    int bad_cfg = wdt_configure(&w, 0);
    if (e_dis == 0 && k_dis == 0 && bad_cfg == 0 && wdt_get_counter(&w) == 0) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected e=0 k=0 cfg=0 cnt=0, got e=%d k=%d cfg=%d cnt=%u)\n", e_dis, k_dis, bad_cfg, wdt_get_counter(&w));

    return 0;
}
