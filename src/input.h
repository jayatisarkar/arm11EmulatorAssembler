
#ifndef ARM11_15_INPUT_H
#define ARM11_15_INPUT_H

#include "utils.h"

/**
 * Handles the reading and writing of binary files.
 */

void readBinaryFile(char *fileName, uint8_t *memory);

void writeBinaryFile(char *fileName, uint32_t *instructions, int numInstr);

#endif //ARM11_15_INPUT_H
