
#ifndef ARM11_15_EXECUTE_H
#define ARM11_15_EXECUTE_H

#include "utils.h"

/**
 * Executes each of the instructions for the emulator.
 */

enum status executeBranch(uint32_t *registers);

void executeDataProcessing(uint32_t *registers);

void multiply(uint32_t *registers);

void singleDataTransfer(uint32_t *registers, uint8_t *mem);


#endif //ARM11_15_EXECUTE_H
