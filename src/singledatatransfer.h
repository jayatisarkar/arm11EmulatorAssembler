
#ifndef ARM11_15_SINGLEDATATRANSFER_H
#define ARM11_15_SINGLEDATATRANSFER_H

/**
 * Handles the single data transfer instruction for both
 * the assembler and emulator.
 */

#include "utils.h"

extern int numVals;

enum sdtCases {
  EXPR,
  BASEREG,
  POST_BASEREG_EXPR,
  PRE_BASEREG_EXPR,
  OPTIONALONE,
  OPTIONALTWO
};

void writeMem(uint32_t address, uint32_t reg, uint8_t *mem);

uint32_t getDestReg(char *instr);

uint32_t getBaseReg(char *addr);

uint32_t generalSDTBinCode(char *instr);

enum sdtCases chooseCase(char *addr);

uint32_t numConst(char *instruction, uint32_t addr, uint32_t bin);

uint32_t preBaseReg(char *instruction, uint32_t addr, uint32_t bin);

uint32_t preBaseRegWExpr(char *instruction, uint32_t addr, uint32_t bin);

uint32_t postBaseRegWExpr(char *instruction, uint32_t addr, uint32_t bin);

uint32_t optionalCases(char *instruction, uint32_t bin, int pBit);

uint32_t ldrAssembler(char *instruction, uint32_t addr);

uint32_t strAssembler(char *instruction, uint32_t addr);

uint32_t ldrAssembler(char *instruction, uint32_t address);

uint32_t strAssembler(char *instruction, uint32_t address);

#endif //ARM11_15_SINGLEDATATRANSFER_H