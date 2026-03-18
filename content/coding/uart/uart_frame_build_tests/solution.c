#include <stdint.h>

uint16_t uart_build_frame(uint8_t data) {
    uint16_t frame = 0;
    // Bit 0: start bit = 0 (already 0)
    // Bits 1-8: data
    frame |= ((uint16_t)data << 1);
    // Bit 9: even parity
    int ones = 0;
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i)) ones++;
    }
    if (ones % 2 \!= 0) frame |= (1 << 9);
    // Bit 10: stop bit = 1
    frame |= (1 << 10);
    return frame;
}
