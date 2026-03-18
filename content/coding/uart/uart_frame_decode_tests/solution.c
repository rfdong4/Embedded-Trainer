#include <stdint.h>

uint8_t uart_decode_frame(uint16_t frame) {
    uint8_t start = frame & 1;
    uint8_t data = (frame >> 1) & 0xFF;
    uint8_t parity = (frame >> 9) & 1;
    uint8_t stop = (frame >> 10) & 1;
    if (start != 0) return 0xFF;
    if (stop != 1) return 0xFF;
    int ones = 0;
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i)) ones++;
    }
    if ((ones + parity) % 2 != 0) return 0xFF;
    return data;
}
