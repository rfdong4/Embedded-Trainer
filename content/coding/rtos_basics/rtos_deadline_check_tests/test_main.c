#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "solution.c"

int main(void) {
    /* TEST 1: Single task, U=0.5, bound=1.0 => schedulable */
    Task t1[] = {{100, 50}};
    float u1 = compute_utilization(t1, 1);
    int s1 = is_schedulable(t1, 1);
    if (fabsf(u1 - 0.5f) < 0.001f && s1 == 1) printf("TEST 1: PASS\n");
    else printf("TEST 1: FAIL (expected U=0.500 sched=1, got U=%.3f sched=%d)\n", u1, s1);

    /* TEST 2: Two tasks, U=0.5+0.25=0.75, bound~0.828 => schedulable */
    Task t2[] = {{100, 50}, {200, 50}};
    float u2 = compute_utilization(t2, 2);
    int s2 = is_schedulable(t2, 2);
    if (fabsf(u2 - 0.75f) < 0.001f && s2 == 1) printf("TEST 2: PASS\n");
    else printf("TEST 2: FAIL (expected U=0.750 sched=1, got U=%.3f sched=%d)\n", u2, s2);

    /* TEST 3: Three tasks, U=0.5+0.25+0.1=0.85 > bound~0.780 => not schedulable */
    Task t3[] = {{100, 50}, {200, 50}, {500, 50}};
    float u3 = compute_utilization(t3, 3);
    int s3 = is_schedulable(t3, 3);
    if (fabsf(u3 - 0.85f) < 0.001f && s3 == 0) printf("TEST 3: PASS\n");
    else printf("TEST 3: FAIL (expected U=0.850 sched=0, got U=%.3f sched=%d)\n", u3, s3);

    /* TEST 4: Three tasks with low utilization => schedulable */
    Task t4[] = {{50, 5}, {100, 10}, {200, 20}};
    float u4 = compute_utilization(t4, 3);
    int s4 = is_schedulable(t4, 3);
    if (fabsf(u4 - 0.3f) < 0.001f && s4 == 1) printf("TEST 4: PASS\n");
    else printf("TEST 4: FAIL (expected U=0.300 sched=1, got U=%.3f sched=%d)\n", u4, s4);

    return 0;
}
