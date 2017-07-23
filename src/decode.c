
#include "decode.h"

/**
 * Decodes the 32 bit instruction and stores the value of each field in the
 * instruction in structs created for 4 different instructions to be executed
 * @param instr 32-bit instruction
 */

void decodeDataProc(uint32_t instr) {
  dataPInstr.condition = getBits(instr, COND_START, COND_LENGTH);
  dataPInstr.i = getBits(instr, I_START, FLAG_LENGTH);
  dataPInstr.opcode = getBits(instr, OP_START, OP_LENGTH);
  dataPInstr.s = getBits(instr, SET_START, FLAG_LENGTH);
  dataPInstr.rn = getBits(instr, RN_START, REG_LENGTH);
  dataPInstr.rd = getBits(instr, RD_START, REG_LENGTH);
  dataPInstr.opcodetwo = getBits(instr, OPTWO_START, OPTWO_LENGTH);
}

void decodeMultiply(uint32_t instr) {
  multiplyInstr.condition = getBits(instr, COND_START, COND_LENGTH);
  multiplyInstr.accumulate = getBits(instr, ACC_START, FLAG_LENGTH);
  multiplyInstr.set = getBits(instr, SET_START, FLAG_LENGTH);
  multiplyInstr.rD = getBits(instr, DESTREG_START, REG_LENGTH);
  multiplyInstr.rN = getBits(instr, REGN_START, REG_LENGTH);
  multiplyInstr.rS = getBits(instr, REGS_START, REG_LENGTH);
  multiplyInstr.rM = getBits(instr, REGM_START, REG_LENGTH);
}

void decodeSDT(uint32_t instr) {
  singleDataTransferInstr.condition = getBits(instr, COND_START, COND_LENGTH);
  singleDataTransferInstr.i = getBits(instr, I_START, FLAG_LENGTH);
  singleDataTransferInstr.p = getBits(instr, P_START, FLAG_LENGTH);
  singleDataTransferInstr.u = getBits(instr, U_START, FLAG_LENGTH);
  singleDataTransferInstr.l = getBits(instr, L_START, FLAG_LENGTH);
  singleDataTransferInstr.rn = getBits(instr, RN_START, REG_LENGTH);
  singleDataTransferInstr.rd = getBits(instr, RD_START, REG_LENGTH);
  singleDataTransferInstr.offset = getBits(instr, OFFSET_START,
                                           SDT_OFFSET_LENGTH);
  singleDataTransferInstr.rm = getBits(instr, OFFSET_START, REG_LENGTH);
}

void decodeBranch(uint32_t instr) {
  branchInstruc.condition = getBits(instr, COND_START, COND_LENGTH);
  branchInstruc.offset = getBits(instr, OFFSET_START, OFFSET_LENGTH);
}
