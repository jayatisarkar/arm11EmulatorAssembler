
#include "utils.h"
#include "dataprocessing.h"
#include "input.h"
#include "map.h"
#include "singledatatransfer.h"
#include "multiply.h"
#include "branch.h"

/**
 * Executes the assembler.
 */

int numInstr = 0;
uint32_t instr;

/**
 * Searches for the corresponding opcode in the opcode symbol table
 * @param opName The opcode
 * @param symbolTable The opcode sybmol table
 * @return The struct containing the opcode and the
 * corresponding function pointer
 */
opcodeMap *opcodeSearch(char *opName, struct opcodeMap *symbolTable) {
  for (int i = 0; i < NUM_OPCODES; ++i) {
    if (strcmp(opName, (symbolTable + i)->opcode) == 0) {
      return symbolTable + i;
    }
  }
  return symbolTable;
}

/**
 * To construct an opcode symbol table with the opcode and corresponding
 * function pointer which takes a character pointer to the instruction itself
 * and the current address of the instruction we are reading from the file
 * @param mapPtr Stores the opcode symbol table
 * @return A pointer to a struct that contains the symbol table
 */
opcodeMap *opcodeSymbolTable(struct opcodeMap *mapPtr) {

  char *opNames[] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst",
                     "teq", "cmp",
                     "mul", "mla", "ldr", "str", "beq", "bne", "bge", "blt",
                     "bgt", "ble", "b", "lsl", "andeq"};
  uint32_t (*(funcPtrs[]))(char *instr, uint32_t address) = {&addAssembler,
                                                             &subAssembler,
                                                             &rsbAssembler,
                                                             &andAssembler,
                                                             &eorAssembler,
                                                             &orrAssembler,
                                                             &movAssembler,
                                                             &tstAssembler,
                                                             &teqAssembler,
                                                             &cmpAssembler,
                                                             &mulAssembler,
                                                             &mlaAssembler,
                                                             &ldrAssembler,
                                                             &strAssembler,
                                                             &beqAssembler,
                                                             &bneAssembler,
                                                             &bgeAssembler,
                                                             &bltAssembler,
                                                             &bgtAssembler,
                                                             &bleAssembler,
                                                             &bAssembler,
                                                             &lslAssembler,
                                                             &andeqAssembler};

  for (int i = 0; i < NUM_OPCODES; ++i) {
    (mapPtr + i)->opcode = opNames[i];
    (mapPtr + i)->funcPtr = funcPtrs[i];
  }
  return mapPtr;
}

/**
 * Executes the first and second pass of the assembler by reading the file and
 * creating a symbol table for associating labels with their corresponding
 * memory addresses as well as a opcode symbol table and generates the
 * corresponding binary encoding of that instruction and writes it to the file
 * @param argc Number of command line arguments
 * @param argv Array of strings
 * @return Success or failure
 */
int main(int argc, char **argv) {

  if (argc < 3) {
    printf("Enter two command line arguments\n");
    exit(EXIT_FAILURE);
  }

  char inpFileName[ARRAY_SIZE];
  char binFileName[ARRAY_SIZE];
  FILE *inpFp;
  FILE *binFp;

  strcpy(inpFileName, argv[1]);
  strcpy(binFileName, argv[2]);

  inpFp = fopen(inpFileName, "r");
  if (inpFp == NULL) {
    printf("Error opening input file [%s]\n", inpFileName);
    exit(EXIT_FAILURE);
  }

  binFp = fopen(binFileName, "w+");
  if (binFp == NULL) {
    printf("Error opening output file [%s]\n", binFileName);
    exit(EXIT_FAILURE);
  }

  fillLabelMap(inpFileName);

  char *line = NULL;
  size_t len = 0;
  int instrWithVal;
  uint32_t currentAddress = 0;
  uint32_t *instructions;
  char *savePtr;

  instructions = malloc(ARRAY_SIZE * sizeof(uint32_t));

  if (instructions == NULL) {
    perror("instructions_malloc");
    exit(EXIT_FAILURE);
  }

  opcodeMap *symbolTable = malloc(NUM_OPCODES * sizeof(opcodeMap));

  if (symbolTable == NULL) {
    perror("symbolTable_malloc");
    exit(EXIT_FAILURE);
  }

  symbolTable = opcodeSymbolTable(symbolTable);

  //reads a file line by line
  while (getline(&line, &len, inpFp) != -1) {
    //skips blank lines
    if (line[0] == '\n') {
      continue;
    }
    if (!isLabel(line)) {

      char *opcode = NULL;
      opcode = strtok_r(line, " ", &savePtr);
      //finds the opcodeMap struct corresponding to opcode of instruction
      opcodeMap *opcodeStruct = opcodeSearch(opcode, symbolTable);
      //stores the binary encoded instruction in the instr global variiable
      instr = opcodeStruct->funcPtr(savePtr, currentAddress);
      //increments the current address of the instruction by 4
      currentAddress += BYTES_PER_INSTR;
      //stores each line(instruction) in an array
      instructions[numInstr] = instr;
      numInstr++;
    }
  }
  //calculates the total size of the array which holds instruction to be
  // written to the file
  instrWithVal = numInstr + numVals;
  //adds the constant values to the end of the instructions array
  for (int i = 0; i < numVals; i++) {
    instructions[numInstr + i] = endValues[i];
  }
  writeBinaryFile(binFileName, instructions, instrWithVal);
  fclose(inpFp);
  fclose(binFp);
  free(line);
  freeLabels();
  freeLabelMap();
  free(instructions);
  free(symbolTable);

  return EXIT_SUCCESS;
}


