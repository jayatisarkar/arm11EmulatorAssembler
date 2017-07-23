
#ifndef ARM11_15_EMULATE_H
#define ARM11_15_EMULATE_H

#include "utils.h"
#include "outputReg.h"
#include "dataprocessing.h"
#include "singledatatransfer.h"
#include "input.h"
#include "execute.h"
#include "decode.h"

/**
 * Executes the emulator.
 */

uint32_t fetch(void);

void decodeInstruction(uint32_t instr);

enum status executeInstr(void);

void pipeLine(void);

int main(int argc, char **argv);


#endif //ARM11_15_EMULATE_H