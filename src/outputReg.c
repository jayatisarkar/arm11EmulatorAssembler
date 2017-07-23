
#include "outputReg.h"

/**
 * Prints the current state of the registers in memory
 * @param registers Pointer to registers in the memory
 */
void printRegisters(uint32_t *registers) {
  printf("Registers:\n");
  for (size_t i = 0; i < REGS_TO_PRINT; i++) {
    printf("$%-3zu: %10d (0x%08x)\n", i, registers[i], registers[i]);
  }
  printf("PC  : %10d (0x%08x)\n", registers[PC], registers[PC]);
  printf("CPSR: %10d (0x%08x)\n", registers[CPSR], registers[CPSR]);
}

/**
 * Changes the 32 bit instruction from big endian to little endian
 * @param memory 32 bit instruction
 * @return 32 bit instruction in little endian format
 */
uint32_t changeEndianess(uint32_t memory) {
  memory = ((memory >> BYTE3TO0) & MASK1) | ((memory << BYTE1TO2) & MASK2) |
           ((memory >> BYTE2TO1) & MASK3) | ((memory << BYTE0TO3) & MASK4);
  return memory;
}

/**
 * This function prints all values in memory which aren't zero
 * @param mem the pointer to the first memory location
 */
void printNonzeroMem(uint8_t *mem) {
  printf("Non-zero memory:\n");
  for (uint32_t i = 0; i < MEMORY_SIZE; i += BYTES_PER_INSTR) {
    uint32_t word = retrieveWord(i, mem);
    if (word) {
      printf("0x%08x: 0x%08x\n", i, changeEndianess(word));
    }
  }
}