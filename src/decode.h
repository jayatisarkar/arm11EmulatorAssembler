
#ifndef ARM11_15_DECODE_H
#define ARM11_15_DECODE_H

#include "utils.h"

/**
 * Splits each of the instructions into their various parts for use by
 * the emulator.
 */

void decodeDataProc(uint32_t instr);

void decodeMultiply(uint32_t instr);

void decodeSDT(uint32_t instr);

void decodeBranch(uint32_t instr);

#endif //ARM11_15_DECODE_H
