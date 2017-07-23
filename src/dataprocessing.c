
#include "dataprocessing.h"

/**
 * Executes respective functions according to the OpCode
 * @param reg First operand register
 * @param op Second operand
 * @return Result to be stored in the destination register
 */
uint32_t and(uint32_t reg, uint32_t op) {
  return reg & op;
}

uint32_t eor(uint32_t reg, uint32_t op) {
  return reg ^ op;
}

uint32_t sub(uint32_t reg, uint32_t op) {
  return reg - op;
}

uint32_t add(uint32_t reg, uint32_t op) {
  return reg + op;
}

uint32_t orr(uint32_t reg, uint32_t op) {
  return reg | op;
}

/**
 * Gets the last bit from the instruction
 * @param result 32 bit instruction
 * @return The last bit extended to 32 bits
 */
uint32_t getLastBit(uint32_t result) {
  return getBits(result, LAST_BIT, ONE_BIT);
}

/**
 * Selects which data processing instruction to execute  and
 * sets CPSR flags if needed
 * @param registers Points to registers in the memory
 * @param instruction 32-bit instruction
 */
void dataProcessing(uint32_t *registers, uint32_t instruction) {
  uint32_t result;
  uint32_t tempCarry;
  switch (dataPInstr.opcode) {
    case AND_OPCODE:
      result = and(registers[dataPInstr.rn], instruction);
      tempCarry = carryout;
      registers[dataPInstr.rd] = result;
      break;
    case EOR_OPCODE:
      result = eor(registers[dataPInstr.rn], instruction);
      tempCarry = carryout;
      registers[dataPInstr.rd] = result;
      break;
    case ORR_OPCODE:
      result = orr(registers[dataPInstr.rn], instruction);
      tempCarry = carryout;
      registers[dataPInstr.rd] = result;
      break;
    case ADD_OPCODE:
      result = add(registers[dataPInstr.rn], instruction);
      tempCarry = getLastBit(result);
      registers[dataPInstr.rd] = result;
      break;
    case SUB_OPCODE:
      result = sub(registers[dataPInstr.rn], instruction);
      tempCarry = (uint32_t) (getLastBit(result) == 0);
      registers[dataPInstr.rd] = result;
      break;
    case RSB_OPCODE:
      result = sub(instruction, registers[dataPInstr.rn]);
      tempCarry = (uint32_t) (getLastBit(result) == 0);
      registers[dataPInstr.rd] = result;
      break;
    case TST_OPCODE:
      result = and(registers[dataPInstr.rn], instruction);
      tempCarry = carryout;
      break;
    case TEQ_OPCODE:
      result = eor(registers[dataPInstr.rn], instruction);
      tempCarry = carryout;
      break;
    case CMP_OPCODE:
      result = sub(registers[dataPInstr.rn], instruction);
      tempCarry = (uint32_t) (getLastBit(result) == 0);
      break;
    case MOV_OPCODE:
      result = instruction;
      tempCarry = 0;
      registers[dataPInstr.rd] = result;
      break;
    default:
      return;
  }

  //if the S bit is 1
  if (dataPInstr.s) {
    registers[CPSR] = (uint32_t) (result == 0) << Z;
    registers[CPSR] |= getLastBit(result) << N;
    registers[CPSR] |= tempCarry << C;
  }
}

/**
 * Accepts the instruction as a string and calculates the binary
 * format by parsing the string and executing the data processing
 * instruction according to the given opcode
 * @param instruction The instruction to be converted to binary
 * @param address Keeps track of the current address
 * of the instruction
 * @return Binary form of the instruction
 */

uint32_t andAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *destReg = strtok_r(instruction, ",", &savePtr);
  uint32_t destRegIdx = readRegisterIndex(destReg);
  char *regN = strtok_r(NULL, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (AND << OP_START) + (regNIdx << RN_START) +
          (destRegIdx << RD_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t eorAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *destReg = strtok_r(instruction, ",", &savePtr);
  uint32_t destRegIdx = readRegisterIndex(destReg);
  char *regN = strtok_r(NULL, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (EOR << OP_START) + (regNIdx << RN_START) +
          (destRegIdx << RD_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t subAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *destReg = strtok_r(instruction, ",", &savePtr);
  uint32_t destRegIdx = readRegisterIndex(destReg);
  char *regN = strtok_r(NULL, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (SUB << OP_START) + (regNIdx << RN_START) +
          (destRegIdx << RD_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t rsbAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *destReg = strtok_r(instruction, ",", &savePtr);
  uint32_t destRegIdx = readRegisterIndex(destReg);
  char *regN = strtok_r(NULL, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (RSB << OP_START) + (regNIdx << RN_START) +
          (destRegIdx << RD_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t addAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *destReg = strtok_r(instruction, ",", &savePtr);
  uint32_t destRegIdx = readRegisterIndex(destReg);
  char *regN = strtok_r(NULL, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (ADD << OP_START) + (regNIdx << RN_START) +
          (destRegIdx << RD_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t orrAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *destReg = strtok_r(instruction, ",", &savePtr);
  uint32_t destRegIdx = readRegisterIndex(destReg);
  char *regN = strtok_r(NULL, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (ORR << OP_START) + (regNIdx << RN_START) +
          (destRegIdx << RD_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t tstAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *regN = strtok_r(instruction, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (TST << OP_START) + (1 << SET_START) +
          (regNIdx << RN_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t teqAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *regN = strtok_r(instruction, " ,", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (TEQ << OP_START) + (1 << SET_START) +
          (regNIdx << RN_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t cmpAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *regN = strtok_r(instruction, ",", &savePtr);
  uint32_t regNIdx = readRegisterIndex(regN);
  instr = DATAPROC_INSTR + (CMP << OP_START) + (1 << SET_START) +
          (regNIdx << RN_START);
  readOperand2(savePtr);
  return instr;
}

uint32_t lslAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  uint32_t regN = readRegisterIndex(strtok_r(instruction, ",", &savePtr));
  char *expr = strtok_r(NULL, ",", &savePtr);
  expr++;

  instr = DATAPROC_INSTR + (MOV << OP_START) + (regN << RD_START) + regN;

  uint32_t cnst = (uint32_t) strtol(expr, NULL, 0);
  cnst &= MAX_CONST;
  instr += cnst << SHIFT_INT;
  return instr;
}

uint32_t andeqAssembler(char *instruction, uint32_t address) {
  return 0;
}
