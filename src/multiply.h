
#ifndef ARM11_15_MULTIPLY_H
#define ARM11_15_MULTIPLY_H

#include "utils.h"

/**
 * Handles the multiply instruction for the assembler.
 */

uint32_t generalMulAssembler(char *instruction, uint32_t address);

uint32_t mulAssembler(char *instruction, uint32_t address);

uint32_t mlaAssembler(char *instruction, uint32_t address);

#endif //ARM11_15_MULTIPLY_H