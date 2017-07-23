
#ifndef ARM11_15_OUTPUTREG_H
#define ARM11_15_OUTPUTREG_H

#include "utils.h"

/**
 * Handles the print output for the emulator.
 */

void printRegisters(uint32_t *registers);

void printNonzeroMem(uint8_t *mem);

uint32_t changeEndianess(uint32_t memory);

#endif //ARM11_15_OUTPUTREG_H