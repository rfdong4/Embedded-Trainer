#include <stdint.h>

uint32_t timer_overflow_freq(uint32_t sys_clk, uint16_t prescaler, uint16_t reload) {
    return sys_clk / ((uint32_t)prescaler * (uint32_t)reload);
}
