#include <stdint.h>

uint32_t calc_dead_time_pwm(uint16_t period, uint16_t duty_permille, uint16_t dead_ticks) {
    uint32_t on_ticks = ((uint32_t)period * duty_permille) / 1000;
    uint16_t ch1_compare = (uint16_t)(on_ticks - dead_ticks);
    uint16_t ch2_compare = (uint16_t)(on_ticks + dead_ticks);
    return ((uint32_t)ch1_compare << 16) | ch2_compare;
}
