#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    Watchdog wdt;

    /* TEST 1: Watchdog does not expire before timeout */
    wdt_init(&wdt, 1000);
    uint8_t result = wdt_check(&wdt, 500);
    if (result == 0 && wdt.expired == 0) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected not expired, got result=%u expired=%u)\n", result, wdt.expired);

    /* TEST 2: Watchdog expires when counter reaches timeout */
    wdt_init(&wdt, 1000);
    wdt_check(&wdt, 500);
    result = wdt_check(&wdt, 500);
    if (result == 1 && wdt.expired == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected expired, got result=%u expired=%u)\n", result, wdt.expired);

    /* TEST 3: Feeding the watchdog resets the counter */
    wdt_init(&wdt, 1000);
    wdt_check(&wdt, 800);
    wdt_feed(&wdt);
    result = wdt_check(&wdt, 800);
    if (result == 0 && wdt.expired == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected not expired after feed, got result=%u expired=%u)\n", result, wdt.expired);

    /* TEST 4: Watchdog expires if not fed after exceeding timeout */
    wdt_init(&wdt, 100);
    result = wdt_check(&wdt, 150);
    if (result == 1 && wdt.expired == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected expired on single large elapsed, got result=%u expired=%u)\n", result, wdt.expired);

    return 0;
}
