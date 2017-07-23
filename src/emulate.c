
#include "emulate.h"

enum instructionSet instructionType;
enum status stat;

/**
 * Holds the current state of the ARM machine
 */
struct {
  uint8_t memory[MEMORY_SIZE];
  uint32_t registers[NO_OF_REGS];
} current;

/**
 * Gets the next instruction from the memory
 * @return The next instruction
 */
uint32_t fetch(void) {
  return retrieveWord(current.registers[PC], current.memory);
}

/**
 * Decodes the type of the 32-bit instruction and
 * calls the appropriate function
 * @param instr Current instruction
 */
void decodeInstruction(uint32_t instr) {
  instructionType = determineInstrType(instr);
  switch (instructionType) {
    case DATAPROCESSING:
      decodeDataProc(instr);
      break;
    case MULTIPLY:
      decodeMultiply(instr);
      break;
    case SINGLEDATATRANSFER:
      decodeSDT(instr);
      break;
    case BRANCH:
      decodeBranch(instr);
      break;
    default:
      break;
  }
}

/**
 * Executes the current instruction according to the type of the instruction
 * @return State of the pipeline
 */
enum status executeInstr(void) {
  switch (instructionType) {
    case DATAPROCESSING:
      executeDataProcessing(current.registers);
      return EXECUTE;
    case MULTIPLY:
      multiply(current.registers);
      return EXECUTE;
    case SINGLEDATATRANSFER:
      singleDataTransfer(current.registers, current.memory);
      return EXECUTE;
    case BRANCH:
      return executeBranch(current.registers);
    default:
      return INVALID_;
  }
}

/**
 * Simulates the fetch-execute cycle
 */
void pipeLine(void) {
  stat = CLEAR_PIPELINE;
  uint32_t inst = 0;
  while (1) {
    if (stat == EXECUTE) {
      stat = executeInstr();
    }
    if (stat == INVALID_) {
      break;
    }
    if ((stat == EXECUTE) | (stat == DECODE)) {
      decodeInstruction(inst);
      stat = EXECUTE;
    }
    inst = fetch();
    stat = (stat == EXECUTE) ? stat : DECODE;
    current.registers[PC] += BYTES_PER_INSTR;
  }
}

/**
 * Executes the emulator for the ARM machine
 * @param argc Number of parameters passed
 * @param argv An array of the command line arguments
 * @return Whether the execution has succeeded or failed
 */
int main(int argc, char **argv) {

  if (argc < 2) {
    printf("Enter one command line argument\n");
    exit(EXIT_FAILURE);
  }

  //initialises the memory and the registers array
  for (int i = 0; i < MEMORY_SIZE; i++) {
    *(current.memory + i) = 0;
  }

  for (int i = 0; i < NO_OF_REGS; i++) {
    *(current.registers + i) = 0;
  }

  size_t fileNameLength = strlen(argv[1]);
  char fileName[fileNameLength];
  strcpy(fileName, argv[1]);
  readBinaryFile(fileName, current.memory);

  //initiates the 3 stage pipeline
  pipeLine();

  printRegisters(current.registers);
  printNonzeroMem(current.memory);

  return EXIT_SUCCESS;
}