#include <stdint.h>

int uart_escape_decode(const uint8_t *input, int input_len, uint8_t *output) {
    int out_idx = 0;
    int i = 0;
    while (i < input_len) {
        if (input[i] == 0x7D) {
            /* Escape character: next byte is XOR'd with 0x20 */
            i++;
            output[out_idx++] = input[i] ^ 0x20;
        } else {
            output[out_idx++] = input[i];
        }
        i++;
    }
    return out_idx;
}
