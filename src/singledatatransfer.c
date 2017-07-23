
#include "singledatatransfer.h"

int numVals = 0;
int numofLdrInstr = 0;
uint32_t endValues[ARRAY_SIZE];

/**
 * Checks if the given address is within the range of the addresses
 * of the GPIO pins and then prints out when pins are enabled, disabled,
 * turned on and turned off. Otherwise, stores the value that
 * the source register holds into the memory at the given address
 * @param address Memory address where the word is to be stored
 * @param reg Source register that holds the word to be stored into memory
 * @param mem Array that holds the memory
 */
void writeMem(uint32_t address, uint32_t reg, uint8_t *mem) {

  if (address >= GPIO0TO9) {
    if (address == GPIO0TO9 || address == GPIO10TO19 ||
        address == GPIO20TO29) {
      char *s;
      switch (address) {
        case GPIO0TO9:
          s = "0 to 9";
          break;
        case GPIO10TO19:
          s = "10 to 19";
          break;
        default:
          s = "20 to 29";
      }
      printf("One GPIO pin from %s has been accessed\n", s);
    }

    if (address == GPIOCLEAR) {
      printf("PIN OFF\n");
    }

    if (address == GPIOSET) {
      printf("PIN ON\n");
    }
  } //if current address does not exceed memory's maximum size
  else if (!notInRange(address, MEMORY_SIZE - BYTES_PER_INSTR)) {
    for (int i = 0; i < BYTES_PER_INSTR; i++) {
      mem[address + i] = (uint8_t) getBits(reg, BITS_PER_BYTE * i,
                                           BITS_PER_BYTE);
    }
  }
}

/**
 * Gets the address of the instruction by parsing the string.
 * @param instruction The instruction as a character pointer
 * @return The address
 */
char *allAddr(char *instruction) {
  char *addr;
  addr = strchr(instruction, ',');
  addr++;
  return addr;
}

/**
 * Parses the string according to the given delimiter and returns
 * the expression
 * @param addr Address from the given instruction
 * @param delimiter The delimiter that helpes parse the string
 * @return The expression
 */
char *getExpr(char *addr, char delimiter) {
  char *expr = strchr(addr, delimiter);
  expr++;
  return expr;
}

/**
 * Calculate the binary index of the destination register from the
 * address of the given instruction
 * @param addr Address of the given instruction
 * @return The index of the destination register in binary form
 */
uint32_t getDestReg(char *addr) {
  char *destReg;
  destReg = strchr(addr, 'r');
  destReg++;
  return (uint32_t) strtol(destReg, NULL, 0);
}

/**
 * Calculate the binary index of the base register from the
 * address of the given instruction
 * @param addr Address of the given instruction
 * @return The index of the base register in binary form
 */
uint32_t getBaseReg(char *addr) {
  char *baseReg;
  baseReg = strchr(addr, 'r');
  baseReg++;
  return (uint32_t) strtol(baseReg, NULL, 0);
}

/**
 * Generates the binary form of the condition code, index of the
 * destination register and the constant bits 26 to 27 of the SDT
 * instruction and stores it in a 32 bit integer
 * @param instr The given instruction
 * @return 32 bit integer
 */
uint32_t generalSDTBinCode(char *instr) {
  uint32_t generalSDTBinary = 0;
  generalSDTBinary |= generateMask(CONDITION, COND_BITS);
  generalSDTBinary |= (getDestReg(instr) << DEST_REG_DISP);
  generalSDTBinary |= generateMask(CONSTANT, 1);
  return generalSDTBinary;
}

/**
 * Parses the address of the given SDT instruction and determines
 * which case is executed by calculating the number of #, commas,
 * r or = in the string
 * @param addr The address of the given instruction
 * @return An enum representing the case that is executed
 */
enum sdtCases chooseCase(char *addr) {
  enum sdtCases sdtcase;
  int hash = 0;
  int equal = 0;
  int comma = 0;
  int numofR = 0;
  char lastChar = addr[strlen(addr) - 2];

  for (int i = 0; addr[i] != '\0'; i++) {
    if (addr[i] == '#') {
      hash++;
    }
    if (addr[i] == ',') {
      comma++;
    }

    if (addr[i] == 'r') {
      numofR++;
    }
    if (addr[i] == '=') {
      equal++;
    }
  }

  if (equal == 1) {
    sdtcase = EXPR;
  } else if (numofR == 1 && comma == 0) {
    sdtcase = BASEREG;
  } else if (numofR == 1 && hash == 1 && lastChar == ']') {
    sdtcase = PRE_BASEREG_EXPR;
  } else if (numofR >= 2 && lastChar == ']') {
    sdtcase = OPTIONALTWO;
  } else if (numofR >= 2 && lastChar != ']') {
    sdtcase = OPTIONALONE;
  } else {
    sdtcase = POST_BASEREG_EXPR;
  }
  printf("%d\n", numofR);
  return sdtcase;
}

/**
 * Converts the SDT instruction to a mov expression by replacing the
 * = character in the instruction to a #
 * @param instr The given instruction
 * @return The new instruction with the = character replaced
 */
char *instrToMoveAccessible(char *instr) {
  char *instrPtr = instr;
  for (int i = 0; instrPtr[i] != '\0'; i++) {
    if (instrPtr[i] == '=') {
      instrPtr[i] = '#';
    }
  }
  return instrPtr;
}

/**
 * Generates the binary form of the SDT instruction when the expression is a
 * numeric constant
 * @param instruction The given instruction
 * @param addr Current address of the given instruction
 * @param bin Stores the binary form of the instruction
 * @return Calculated binary form of the SDT instruction
 */
uint32_t numConst(char *instruction, uint32_t addr, uint32_t bin) {
  char *valofExp = strchr(allAddr(instruction), '=');
  valofExp++; //points to the value of the expression
  uint32_t valofExpr = (uint32_t) strtol(valofExp, NULL, 0);

  //if value is less than 0xFF mov instruction is executed
  if (valofExpr <= MAX_CONSTANT) {
    return movAssembler(instrToMoveAccessible(instruction), valofExpr);
  } else {
    uint32_t memAdd = (uint32_t) ((numInstructions + numofLdrInstr) *
                                  BYTES_PER_INSTR);
    printf("%d\n", memAdd);
    endValues[numVals++] = valofExpr;
    uint32_t offset = memAdd - (addr + PC_OFFSET);
    bin |= offset;

    uint32_t baseReg = PC;
    bin |= baseReg << BASE_REG_DISP;
    bin |= generateMask(ADD_DISP, 1);
    bin |= generateMask(P_DISP, 1);

    numofLdrInstr++;
    return bin;
  }
}

/**
 * Generates the binary form of the SDT instruction when it has a pre-indexed
 * address specification with an offset of 0
 * @param instruction The given instruction
 * @param addr Keeps track of the current address of the instruction
 * @param bin Stores the binary form
 * @return Calculated binary form of the SDT instruction
 */
uint32_t preBaseReg(char *instruction, uint32_t addr, uint32_t bin) {
  uint32_t baseReg = getBaseReg(allAddr(instruction));
  bin |= baseReg << BASE_REG_DISP;
  bin |= generateMask(ADD_DISP, 1);
  bin |= generateMask(P_DISP, 1);
  return bin;
}

/**
 * Generates the binary form of the SDT instruction when it has a pre-indexed
 * address specification and also calculates the offset by parsing the string
 * @param instruction The given instruction
 * @param addr Keeps track of the current address of the instruction
 * @param bin Stores the binary form
 * @return Calculated binary form of the SDT instruction
 */
uint32_t preBaseRegWExpr(char *instruction, uint32_t addr, uint32_t bin) {
  char *expr = getExpr(allAddr(instruction), '#');
  if (expr[0] == '-') {
    expr++;
  } else {
    bin |= generateMask(ADD_DISP, 1);
  }
  uint32_t offset = (uint32_t) strtol(expr, NULL, 0);
  bin |= offset;
  uint32_t baseReg = getBaseReg(allAddr(instruction));
  bin |= baseReg << BASE_REG_DISP;
  bin |= generateMask(P_DISP, 1);
  return bin;
}

/**
 * Generates the binary form of the SDT instruction when it has a post-indexed
 * address specification and also calculates the offset by parsing the string
 * @param instruction The given instruction
 * @param addr Keeps track of the current address of the instruction
 * @param bin Stores the binary form
 * @return Calculated binary form of the SDT instruction
 */
uint32_t postBaseRegWExpr(char *instruction, uint32_t addr, uint32_t bin) {
  char *expr = getExpr(allAddr(instruction), '#');
  if (expr[0] == '-') {
    expr++;
  } else {
    bin |= generateMask(ADD_DISP, 1);
  }
  uint32_t offset = (uint32_t) strtol(expr, NULL, 0);
  bin |= offset;
  uint32_t baseReg = getBaseReg(allAddr(instruction));
  bin |= baseReg << BASE_REG_DISP;
  return bin;
}

/**
 * Checks if the given string has commas
 * @param expr Character pointer that represents the expression
 * of the instruction
 * @return True if there is a comma present, false otherwise
 */
bool hasComma(char *expr) {
  bool comma = false;
  for (int i = 0; expr[i] != '\0'; i++) {
    if (expr[i] == ',') {
      comma = true;
    }
  }
  return comma;
}

/**
 * Generates the binary form of the SDT instruction for the optional cases
 * of the pre-indexing and post-indexing specification
 * @param instruction The given instruction
 * @param bin Stores the binary form
 * @param pBit Determines whether it is pre-indexing or post-indexing
 * @return Calculated binary form of the SDT instruction
 */
uint32_t optionalCases(char *instruction, uint32_t bin, int pBit) {
  char *expr = getExpr(allAddr(instruction), ',');
  if (expr[0] == '-') {
    expr++;
    expr++;
  } else if (expr[0] == '+') {
    bin |= generateMask(ADD_DISP, 1);
    expr++;
    expr++;
  } else {
    bin |= generateMask(ADD_DISP, 1);
    expr++;
  }

  uint32_t regM = (uint32_t) strtol(expr, NULL, 0);
  bin |= regM;

  if (hasComma(expr)) {
    expr = getExpr(expr, ',');

    char shiftType[SHIFT_LEN + 1];
    for (int j = 0; j < SHIFT_LEN; j++) {
      shiftType[j] = expr[j];
    }

    if (!strcmp(shiftType, "lsr")) {
      bin |= generateMask(LSR_DISP, 1);
    } else if (!strcmp(shiftType, "asr")) {
      bin |= generateMask(ASR_DISP, 1);
    } else if (!strcmp(shiftType, "ror")) {
      bin |= generateMask(ROR_DISP, TWO_BIT_SET);
    }
    expr += SHIFT_LEN;

    if (expr[0] == '#') {
      expr++;
      uint32_t num = (uint32_t) strtol(expr, NULL, BASE10);
      bin |= num << INT_DISP;
    }

    if (expr[0] == 'r') {
      expr++;
      uint32_t regS = (uint32_t) strtol(expr, NULL, BASE10);
      bin |= regS << REG_S_SHIFT;
      bin |= generateMask(REG_S_DISP, 1);
    }
  }

  uint32_t baseReg = getBaseReg(allAddr(instruction));
  bin |= baseReg << BASE_REG_DISP;
  bin |= generateMask(I_DISP, 1);
  if (pBit) {
    bin |= generateMask(P_DISP, 1);
  }
  return bin;
}

/**
 * Generate the binary format for the ldr instruction by choosing between
 * the different possible cases that can be executed using an enum
 * @param instruction The given instruction
 * @param addr The address of the instruction
 * @return Binary format of the instruction
 */
uint32_t ldrAssembler(char *instruction, uint32_t addr) {

  uint32_t resultBin = generalSDTBinCode(instruction);
  resultBin |= generateMask(LOAD_DISP, 1); //Load Instruction

  char *str1 = instruction;
  char *str2 = instruction;

  while (*str2 != '\0') {
    *str1 = *str2++;
    if (*str1 != ' ') {
      str1++;
    }
  }
  *str1 = 0;

  switch (chooseCase(allAddr(instruction))) {
    case EXPR:
      resultBin = numConst(instruction, addr, resultBin);
      printf("%u\n", resultBin);
      break;
    case BASEREG:
      resultBin = preBaseReg(instruction, addr, resultBin);
      printf("%u\n", resultBin);
      break;
    case PRE_BASEREG_EXPR:
      resultBin = preBaseRegWExpr(instruction, addr, resultBin);
      printf("%u\n", resultBin);
      break;
    case POST_BASEREG_EXPR:
      resultBin = postBaseRegWExpr(instruction, addr, resultBin);
      printf("%u\n", resultBin);
      break;
    case OPTIONALONE:
      resultBin = optionalCases(instruction, resultBin, 0);
      break;
    case OPTIONALTWO:
      resultBin = optionalCases(instruction, resultBin, 1);
      break;
    default:
      break;
  }
  return resultBin;
}

/**
 * Generate the binary format for the str instruction by choosing between
 * the different possible cases that can be executed using an enum
 * @param instruction The given instruction
 * @param addr The address of the instruction
 * @return Binary format of the instruction
 */
uint32_t strAssembler(char *instruction, uint32_t addr) {
  uint32_t resultBin = generalSDTBinCode(instruction);

  char *str1 = instruction;
  char *str2 = instruction;

  while (*str2 != '\0') {
    *str1 = *str2++;
    if (*str1 != ' ') {
      str1++;
    }
  }
  *str1 = 0;

  switch (chooseCase(allAddr(instruction))) {
    case BASEREG:
      resultBin = preBaseReg(instruction, addr, resultBin);
      printf("%d", resultBin);
      break;
    case PRE_BASEREG_EXPR:
      resultBin = preBaseRegWExpr(instruction, addr, resultBin);
      printf("%d", resultBin);
      break;
    case POST_BASEREG_EXPR:
      resultBin = postBaseRegWExpr(instruction, addr, resultBin);
      printf("%d", resultBin);
      break;
    case OPTIONALONE:
      resultBin = optionalCases(instruction, resultBin, 0);
      break;
    case OPTIONALTWO:
      resultBin = optionalCases(instruction, resultBin, 1);
      break;
    default:
      break;
  }
  return resultBin;
}
