#include <stdint.h>

uint16_t uart_build_frame(uint8_t data, uint8_t parity) {
    uint16_t frame = 0;
    /* Bit 0: start bit = 0 (already 0) */
    /* Bits 1-8: data bits */
    frame |= ((uint16_t)data << 1);

    /* Count 1-bits in data */
    int ones = 0;
    for (int i = 0; i < 8; i++) {
        if (data & (1 << i)) ones++;
    }

    if (parity == 0) {
        /* No parity: stop bit at bit 9 */
        frame |= (1 << 9);
    } else {
        /* Calculate parity bit at bit 9 */
        if (parity == 1) {
            /* Even parity: set if odd number of 1s */
            if (ones % 2 != 0) frame |= (1 << 9);
        } else {
            /* Odd parity: set if even number of 1s */
            if (ones % 2 == 0) frame |= (1 << 9);
        }
        /* Stop bit at bit 10 */
        frame |= (1 << 10);
    }

    return frame;
}
