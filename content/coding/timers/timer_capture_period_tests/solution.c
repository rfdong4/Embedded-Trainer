#include <stdint.h>

uint32_t capture_period_us(uint16_t cap1, uint16_t cap2, uint32_t tick_freq_hz) {
    uint32_t elapsed;
    if (cap2 >= cap1) {
        elapsed = cap2 - cap1;
    } else {
        elapsed = 65536 - cap1 + cap2;
    }
    return (uint32_t)(((uint64_t)elapsed * 1000000) / tick_freq_hz);
}
