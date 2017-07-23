
#include "input.h"

/**
 * Opens the file and reads it into the memory 32 bits at a time
 * @param fileName Name of the binary file
 * @param memory Array which stores the instructions
 */
void readBinaryFile(char *fileName, uint8_t *memory) {
  FILE *fp;
  fp = fopen(fileName, "rb");

  if (fp == NULL) {
    perror("readBinaryFile");
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0, SEEK_END);
  int fileLength = (int) ftell(fp);
  rewind(fp);

  size_t numElems = (size_t) fileLength / INSTR_SIZE;
  fread(memory, INSTR_SIZE, numElems, fp);
  fclose(fp);
}

/**
 * Opens the file and writes the 32 bit instruction to the file
 * @param fileName Name of the file to be written to
 * @param instructions The 32 bit instruction
 * @param numInstr The number of instructions to be written to the file
 */
void writeBinaryFile(char *fileName, uint32_t *instructions, int numInstr) {
  FILE *fp;
  fp = fopen(fileName, "w");
  if (fp == NULL) {
    perror("writeBinaryFile");
    exit(EXIT_FAILURE);
  }
  fwrite(instructions, sizeof(uint32_t), (size_t) numInstr, fp);
  fclose(fp);
}

