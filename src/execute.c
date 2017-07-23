
#include "execute.h"
#include "dataprocessing.h"
#include "singledatatransfer.h"

/**
 * Updates the second operand value and executes the data processing instruction
 * under the given condition code
 * @param registers Array of registers from which the CPSR register is accessed
 */
void executeDataProcessing(uint32_t *registers) {
  if (condCheck(dataPInstr.condition, registers[CPSR])) {
    uint32_t opTwo = updateOpTwo(registers, dataPInstr.opcodetwo,
                                 dataPInstr.i);
    dataProcessing(registers, opTwo);
  }
}

/**
 * Executes the instruction under the given condition code by
 * calculating the result depending on the 21st bit whether it is multiply and
 * accumulate or just multiply and stores the result in the destination
 * register.
 * Also updates the CPSR flags according to the 20th bit
 * @param registers Array of registers from which the CPSR register,
 * destination register and operand registers are accessed
 */
void multiply(uint32_t *registers) {
  if (condCheck(multiplyInstr.condition, registers[CPSR])) {
    registers[multiplyInstr.rD] =
      registers[multiplyInstr.rM] * registers[multiplyInstr.rS];

    if (multiplyInstr.accumulate) {
      registers[multiplyInstr.rD] += registers[multiplyInstr.rN];
    }

    //if the S bit is set
    if (multiplyInstr.set) {
      registers[CPSR] = registers[CPSR] |
                        getBits(registers[multiplyInstr.rD], REG_SIZE - 1,
                                1)
                          << N;
      if (!registers[multiplyInstr.rD]) {
        registers[CPSR] = registers[CPSR] | 1 << Z;
      }
    }
  }
}

/**
 * Executes the single data transfer instruction under the given condition code
 * by checking the 23rd, 24th and 25th bit to calculate the 12 bit offset and
 * load from memory or store into memory according to the 20th bit
 * @param registers Array of registers from which the CPSR register, base
 * register and source register are accessed
 * @param mem Array that holds the memory
 */
void singleDataTransfer(uint32_t *registers, uint8_t *mem) {

  uint32_t offsetBits;
  uint32_t baseregister = registers[singleDataTransferInstr.rn];

  if (condCheck(singleDataTransferInstr.condition, registers[CPSR])) {
    //if it is an immediate offset
    if (singleDataTransferInstr.i) {
      singleDataTransferInstr.offset =
        updateOpTwo(registers, singleDataTransferInstr.offset, 0);
    }

    //if the U bit is set
    if (singleDataTransferInstr.u) {
      offsetBits = (singleDataTransferInstr.offset) + baseregister;
    } else {
      offsetBits = baseregister - (singleDataTransferInstr.offset);
    }

    //pre indexing
    if (singleDataTransferInstr.p) {
      if (singleDataTransferInstr.l) {
        registers[singleDataTransferInstr.rd] = retrieveWord(offsetBits,
                                                             mem);
      } else {
        writeMem(offsetBits, registers[singleDataTransferInstr.rd],
                 mem);
      }
    } // post indexing
    else {
      if (singleDataTransferInstr.l) {
        registers[singleDataTransferInstr.rd] = retrieveWord(
          baseregister, mem);
      } else {
        writeMem(baseregister, registers[singleDataTransferInstr.rd],
                 mem);
      }
      registers[singleDataTransferInstr.rn] = offsetBits;
    }
  }
}

/**
 * Executes the branch instruction under the given condition code by receiving
 * the 24 bit offset and adding it to the PC after extending it to 32 bits
 * @param registers Array of registers from which the PC is accessed
 * @return
 */
enum status executeBranch(uint32_t *registers) {
  if (condCheck(branchInstruc.condition, registers[CPSR])) {
    int32_t shiftOffset = branchInstruc.offset;
    shiftOffset <<= OFFSET_SHIFT;
    registers[PC] += shiftOffset;
    return CLEAR_PIPELINE;
  }
  return EXECUTE;
}
