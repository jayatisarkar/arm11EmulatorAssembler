#ifndef ARM11_15_DATAPROCESSING_H
#define ARM11_15_DATAPROCESSING_H

#include "utils.h"

/**
 * Handles the data processing instruction for both the
 * assembler and emulator.
 */

#define AND 0
#define EOR 1
#define SUB 2
#define RSB 3
#define ADD 4
#define TST 8
#define TEQ 9
#define CMP 10
#define ORR 12

enum operation {
  AND_OPCODE = 0,
  EOR_OPCODE = 1,
  SUB_OPCODE = 2,
  RSB_OPCODE = 3,
  ADD_OPCODE = 4,
  TST_OPCODE = 8,
  TEQ_OPCODE = 9,
  CMP_OPCODE = 10,
  ORR_OPCODE = 12,
  MOV_OPCODE = 13
};

uint32_t and(uint32_t reg, uint32_t op);

uint32_t eor(uint32_t reg, uint32_t op);

uint32_t sub(uint32_t reg, uint32_t op);

uint32_t add(uint32_t reg, uint32_t op);

uint32_t orr(uint32_t reg, uint32_t op);

uint32_t getLastBit(uint32_t result);

void dataProcessing(uint32_t *registers, uint32_t instruction);

uint32_t andAssembler(char *instruction, uint32_t address);

uint32_t eorAssembler(char *instruction, uint32_t address);

uint32_t subAssembler(char *instruction, uint32_t address);

uint32_t rsbAssembler(char *instruction, uint32_t address);

uint32_t addAssembler(char *instruction, uint32_t address);

uint32_t orrAssembler(char *instruction, uint32_t address);

uint32_t tstAssembler(char *instruction, uint32_t address);

uint32_t teqAssembler(char *instruction, uint32_t address);

uint32_t cmpAssembler(char *instruction, uint32_t address);

uint32_t lslAssembler(char *instruction, uint32_t address);

uint32_t andeqAssembler(char *instruction, uint32_t address);

#endif //ARM11_15_DATAPROCESSING_H