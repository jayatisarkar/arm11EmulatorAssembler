
#include "multiply.h"

/**
 * Generates the binary encoding of the given multiply instruction
 * @param instruction 32 bit instruction
 * @param address Keeps track of the current address of the instruction
 * @return Binary format of the given instruction
 */
uint32_t generalMulAssembler(char *instruction, uint32_t address) {
  char *destReg = strtok(instruction, ",");
  uint32_t destRegIdx = readRegisterIndex(destReg);
  char *regM = strtok(NULL, ",");
  uint32_t regMIdx = readRegisterIndex(regM);
  char *regS = strtok(NULL, ",");
  uint32_t regSIdx = readRegisterIndex(regS);
  instr = MUL_INSTR + (destRegIdx << DESTREG_START) + (regSIdx << REGS_START)
          + (regMIdx << REGM_START);
  return instr;
}

/**
 * Calls the generalMulAssembler function if the accumulate bit is not set
 * @param instruction 32 bit instruction
 * @param address Current address of the instruction
 * @return Binary format of the given instruction
 */
uint32_t mulAssembler(char *instruction, uint32_t address) {
  return generalMulAssembler(instruction, address);
}

/**
 * Executes the multiply instruction when the accumulate bit is to  be set
 * @param instruction 32 bit instruction
 * @param address Current address of the instruction
 * @return Binary format of the given instruction
 */
uint32_t mlaAssembler(char *instruction, uint32_t address) {
  instr = generalMulAssembler(instruction, address);
  instr = instr | generateMask(ACC, 1);

  char *regN = strtok(NULL, ",");
  uint32_t regNIdx = readRegisterIndex(regN);
  instr += (regNIdx << REGN_START);
  return instr;
}
