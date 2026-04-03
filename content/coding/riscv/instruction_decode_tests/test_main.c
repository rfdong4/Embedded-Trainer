#include <stdio.h>
#include <stdint.h>
#include "solution.c"

int main(void) {
    DecodedInstr d;

    /* TEST 1: ADD x1, x2, x3 -> 0x003100B3
       opcode=0x33, rd=1, funct3=0, rs1=2, rs2=3, funct7=0x00 */
    decode_r_type(0x003100B3, &d);
    if (d.opcode == 0x33 && d.rd == 1 && d.funct3 == 0 &&
        d.rs1 == 2 && d.rs2 == 3 && d.funct7 == 0x00)
        printf("TEST 1: PASS\n");
    else
        printf("TEST 1: FAIL (op=0x%02X rd=%d f3=%d rs1=%d rs2=%d f7=0x%02X)\n",
               d.opcode, d.rd, d.funct3, d.rs1, d.rs2, d.funct7);

    /* TEST 2: SUB x5, x6, x7 -> 0x407302B3
       opcode=0x33, rd=5, funct3=0, rs1=6, rs2=7, funct7=0x20 */
    decode_r_type(0x407302B3, &d);
    if (d.opcode == 0x33 && d.rd == 5 && d.funct3 == 0 &&
        d.rs1 == 6 && d.rs2 == 7 && d.funct7 == 0x20)
        printf("TEST 2: PASS\n");
    else
        printf("TEST 2: FAIL (op=0x%02X rd=%d f3=%d rs1=%d rs2=%d f7=0x%02X)\n",
               d.opcode, d.rd, d.funct3, d.rs1, d.rs2, d.funct7);

    /* TEST 3: AND x10, x11, x12 -> 0x00C5F533
       opcode=0x33, rd=10, funct3=7, rs1=11, rs2=12, funct7=0x00 */
    decode_r_type(0x00C5F533, &d);
    if (d.opcode == 0x33 && d.rd == 10 && d.funct3 == 7 &&
        d.rs1 == 11 && d.rs2 == 12 && d.funct7 == 0x00)
        printf("TEST 3: PASS\n");
    else
        printf("TEST 3: FAIL (op=0x%02X rd=%d f3=%d rs1=%d rs2=%d f7=0x%02X)\n",
               d.opcode, d.rd, d.funct3, d.rs1, d.rs2, d.funct7);

    /* TEST 4: SLL x15, x16, x17 -> 0x011817B3
       opcode=0x33, rd=15, funct3=1, rs1=16, rs2=17, funct7=0x00 */
    decode_r_type(0x011817B3, &d);
    if (d.opcode == 0x33 && d.rd == 15 && d.funct3 == 1 &&
        d.rs1 == 16 && d.rs2 == 17 && d.funct7 == 0x00)
        printf("TEST 4: PASS\n");
    else
        printf("TEST 4: FAIL (op=0x%02X rd=%d f3=%d rs1=%d rs2=%d f7=0x%02X)\n",
               d.opcode, d.rd, d.funct3, d.rs1, d.rs2, d.funct7);

    return 0;
}
