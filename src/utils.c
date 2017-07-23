
#include "utils.h"
#define INSTR_BITS 32
#define MASK_I 0x00000002


bool get_bit(uint32_t x, int bit) {
    return (x & (1 << bit)) != 0;
}


uint32_t returnBit(bool bit) {
    if(bit) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t carryout;
int numInstructions = 0;
uint32_t instr;

/**
 * To determine the type of the instruction according to the bits
 * @param instruction 32 bit instruction
 * @return Type of the instruction
 */
enum instructionSet determineInstrType(uint32_t instruction) {
  if (!instruction) {
    return STOP;
  }
  uint32_t instrDetermineBitOne = getBits(instruction, BIT26, INSTR_BIT_LEN1);
  if (instrDetermineBitOne == INSTR_SDT) {
    return SINGLEDATATRANSFER;
  } else if (instrDetermineBitOne == INSTR_BRANCH) {
    return BRANCH;
  }
  if (instrDetermineBitOne == ZERO) {
    uint32_t instrDetermineBitTwo = getBits(instruction, BIT4,
                                            INSTR_BIT_LEN2);
    if (instrDetermineBitTwo == INSTR_MULT) {
      return MULTIPLY;
    } else {
      return DATAPROCESSING;
    }
  } else {
    return STOP;
  }
}

/**
 * Checks if the given bit is 0 or 1
 * @param x 32 bit instruction
 * @param bit The bit to be checked
 * @return True if the given bit is 1
 */
bool getBit(uint32_t x, int bit) {
  return (x & (1 << bit)) != 0;
}

/**
 * Creates a mask to get consecutive bits from a 32 bit instruction
 * @param startBit The starting bit position from where a mask is created
 * @param numOfOnes The size of the mask from the starting  bit
 * @return The mask extended to 32 bits
 */
uint32_t generateMask(uint32_t startBit, uint32_t numOfOnes) {
  uint32_t result = 0;
  for (int i = 0; i < numOfOnes; i++) {
    result <<= 1;
    result++;
  }
  return result << startBit;
}

/**
 * To get a range of bits from the given instruction
 * @param num 32 bit instruction
 * @param start The position of the starting bit
 * @param length The number of bits to be be calculated
 * from the starting position
 * @return The calculated bits
 */
uint32_t getBits(uint32_t num, int start, int length) {
  uint32_t result = 0;
  for (int i = start; i < start + length; i++) {
    result += (1 << i) & num;
  }
  return result >> start;
}

/**
 * Identifies the conditions under which the operation is executed
 * @param cond Condition code of the ARM instruction
 * @param cpsrReg 32 bit CPSR register
 * @return
 */
bool condCheck(uint32_t cond, uint32_t cpsrReg) {

  uint32_t n = getBits(cpsrReg, N, FLAG_LENGTH);
  uint32_t z = getBits(cpsrReg, Z, FLAG_LENGTH);
  uint32_t v = getBits(cpsrReg, V, FLAG_LENGTH);
  switch (cond) {
    case EQ:
      return z;
    case NE:
      return !z;
    case GE:
      return n == v;
    case LT:
      return n != v;
    case GT:
      return !z && (n == v);
    case LE:
      return z || (n != v);
    case AL:
      return 1;
    default:
      return 0;
  }
}

/**
 * Shifts the bits to the left and fills the right hand side with 0s
 * @param inst The operand extended to 32 bits
 * @param shiftInt The value by which the operand should be shifted
 * @return Shifted operand
 */
uint32_t logicalShiftLeft(uint32_t inst, uint32_t shiftInt) {
  carryout = (shiftInt) ? (uint32_t) (getBit(inst, REG_SIZE - shiftInt)) : 0;
  return inst << shiftInt;
}

/**
 * Shifts the bits to the right and fills the left hand side with 0s
 * @param inst The operand extended to 32 bits
 * @param shiftInt The value by which the operand should be shifted
 * @return Shifted operand
 */
uint32_t logicalShiftRight(uint32_t inst, uint32_t shiftInt) {
  carryout = (shiftInt) ? ((inst & (1 << (shiftInt - 1)) >> (shiftInt - 1)))
                        : 0;
  return inst >> shiftInt;
}

/**
 * Rotates the bits of the operand to the right
 * @param inst 32 bit operand
 * @param shiftInt The value by which the operand should be rotated
 * @return Rotated operand
 */
uint32_t rotateRight(uint32_t inst, uint32_t shiftInt) {
  carryout = (shiftInt) ? ((inst & (1 << (shiftInt - 1)) >> (shiftInt - 1)))
                        : 0;
  uint32_t r;
  uint32_t s;
  if (shiftInt != 0) {
    r = inst >> shiftInt;
    s = inst << (REG_SIZE - shiftInt);
    return (r | s);
  } else {
    return inst;
  }
}

/**
 * Shifts the bits to the right and fills the left hand side with the
 * most significand bits
 * @param instr The operand extended to 32 bits
 * @param shiftInt The value by which the operand should be shifted
 * @return Shifted operand
 */
uint32_t arithShiftRight(uint32_t instr, uint32_t shiftInt) {
  carryout = (shiftInt) ? ((instr & (1 << (shiftInt - 1)) >> (shiftInt - 1)))
                        : 0;
  return instr >> shiftInt | ~(~0U >> shiftInt);
}

/**
 * Checks if the second operand is an immediate value or a register according
 * to the I bit and performs rotations or shifts on this value accordingly
 * @param registers Array of registers
 * @param instruction 32 bit instruction
 * @param i 25th bit which determines the second operand value
 * @return Updated operand value
 */
uint32_t updateOpTwo(uint32_t *registers, uint32_t instruction, uint32_t i) {
  uint32_t value;
  uint32_t optwo;

  if (i) {
    uint32_t imm = getBits(instruction, 0, IMM_LENGTH);
    uint32_t mask = generateMask(0, IMM_LENGTH);
    value = imm & mask;
    uint32_t rotate =
      getBits(instruction, ROTATE_START, ROTATE_LENGTH) << 1;
    optwo = rotateRight(value, rotate);
  } else {
    uint32_t regIndex = getBits(instruction, 0, REG_LENGTH);
    value = registers[regIndex];
    uint32_t shiftType = getBits(instruction, SHIFT_TYPE_START,
                                 SHIFT_TYPE_LENGTH);

    int bitFour = getBits(instruction, FOURTHBIT, 1);

    uint32_t shiftInt;
    uint32_t rs;

    if (bitFour) {
      rs = getBits(instruction, BIT8, REG_LENGTH);
      shiftInt = registers[rs];
    } else {
      shiftInt = getBits(instruction, BIT7, SHIFT_INT_LENGTH);
    }

    switch (shiftType) {
      case LSL:
        optwo = logicalShiftLeft(value, shiftInt);
        break;
      case LSR:
        optwo = logicalShiftRight(value, shiftInt);
        break;
      case ASR:
        optwo = arithShiftRight(value, shiftInt);
        break;
      default:
        optwo = rotateRight(value, shiftInt);
    }
  }
  return optwo;
}

/**
 * Checks if the given address is greater than the largest memory size
 * @param checkNum 32 bit memory address
 * @param bound Largest memory size
 * @return True if the address is out of range, false otherwise
 */
int notInRange(uint32_t checkNum, uint32_t bound) {
  return checkNum > bound;
}

/**
 * Checks if the given address is within the range of the physical
 * address of the GPIO pins
 * @param address 32 bit bit address
 * @return True if its within range, false otherwise
 */
bool isGPIO(uint32_t address) {
  bool check = address >= GPIO0TO9 && address < GPIO20TO29 + BYTES_PER_INSTR;
  if (check) {
    if (address < GPIO10TO19) {
      printf("One GPIO pin from 0 to 9 has been accessed\n");
    } else if (address < GPIO20TO29) {
      printf("One GPIO pin from 10 to 19 has been accessed\n");
    } else {
      printf("One GPIO pin from 20 to 29 has been accessed\n");
    }
  }
  return check;
}

/**
 * Checks if it is a GPIO address and returns the address, otherwise
 * gets the bytes from the memory and converts it into a 32 bit word
 * by shifting the bytes and concatenating them
 * @param address 32 bit memory address
 * @param mem Array which holds the memory
 * @return
 */
uint32_t retrieveWord(uint32_t address, uint8_t *mem) {

  if (isGPIO(address)) {
    return address;
  }

  if (notInRange(address, MEMORY_SIZE - BYTES_PER_INSTR)) {
    printf("Error: Out of bounds memory access at address %#010x\n",
           address);
    return 0;
  }
  uint32_t word = 0;
  word = (mem[address]) | (mem[address + 1] << BITS_PER_BYTE)
         | (mem[address + 2] << BITS_PER_BYTE * 2)
         | (mem[address + 3] << BITS_PER_BYTE * 3);
  return word;
}

/**
 * Determines the shift type according to the given instruction
 * and returns its binary form
 * @param shiftPtr
 * @return The binary form of shift type extended to 32 bits
 */
uint32_t getShiftType(char *shiftPtr) {
  if (strcmp(shiftPtr, "lsl") == 0) {
    return LSL;
  } else if (strcmp(shiftPtr, "lsr") == 0) {
    return LSR;
  } else if (strcmp(shiftPtr, "asr") == 0) {
    return ASR;
  } else {
    return ROR;
  }
}

/**
 * Generates a binary form of the 12 bit operand value for data processing
 * by rotating right an extended 32 bit decimal value
 * @param constant Binary form of the operand value
 */
void getImmAndRotate(uint32_t constant) {
  if (constant == 0) {
    return;
  }
  uint32_t rot = constant;
  uint32_t updateRot;
  uint32_t maxRot = REG_SIZE;

  //if the immediate we have received cannot be represented in 8 bits
  if (rot > MAX_EXP) {
    bool doesNotFit = 0;
    while (rot > MAX_EXP || doesNotFit) {
      updateRot = rotateRight(rot, 2);
      if (doesNotFit && (updateRot > MAX_EXP)) {
        break;
      }
      rot = updateRot;
      doesNotFit = rot <= MAX_EXP;
      maxRot -= 2;
      if (maxRot <= 0) {
        exit(EXIT_FAILURE);
      }
    }
  }
  uint32_t opTwo = rot + (((maxRot % REG_SIZE) / 2) << BITS_PER_BYTE);
  instr += opTwo;
}

/**
 * Calculates the index of the register to be used in an instruction in binary
 * @param token String that represents the register
 * @return Binary index of the register
 */
uint32_t readRegisterIndex(char *token) {
  char *endPtr;
  return (uint32_t) strtol(token + 1, &endPtr, BASE10);
}

/**
 * Reads the instruction by parsing the string and calculates its binary form
 * @param instruction The instruction in the file
 */
void readOperand2(char *instruction) {
  char *savePtr;
  char *op2Token;
  op2Token = strtok_r(instruction, " ,", &savePtr);
  if (*op2Token == '#') { //op2 is an immediate value so I bit set to 1
    ++op2Token; //moves the pointer to the number after the hash
    if (*(op2Token + 1) == 'x') { //if immediate value is a hexadecimal
      getImmAndRotate((uint32_t) strtol(op2Token, NULL, 0));
    } else { //immediate value is a decimal
      uint32_t x = (uint32_t) (strtol(op2Token, NULL, BASE10));
      getImmAndRotate(x);
    }
    instr += (1 << I_START);
  } else { //op2 is a register
    uint32_t regIndex = readRegisterIndex(op2Token);
    instr += regIndex;
    char *shiftOp = strtok_r(NULL, " ,", &savePtr);
    if (shiftOp == NULL) {
      return;
    }

    uint32_t shiftType = getShiftType(shiftOp);

    shiftType <<= (REG_LENGTH + 1);
    instr += shiftType;
    if (*savePtr == '#') {
      savePtr++;
      uint32_t shiftInt;
      if (*(savePtr + 1) == 'x') {
        shiftInt = (uint32_t) strtol(instruction, NULL, 0);
      } else {
        shiftInt = (uint32_t) strtol(instruction, NULL, BASE10);
      }
      shiftInt <<= (REG_LENGTH + 1 + SHIFT_TYPE_LENGTH);
      instr += shiftInt;
    } else {
      uint32_t regS = readRegisterIndex(savePtr);
      regS <<= (REG_LENGTH + 1 + SHIFT_TYPE_LENGTH + 1);
      instr += (1 << REG_LENGTH) + regS;
    }
  }
}

/**
 * Stores the binary form of the given operand
 * @param instruction The instruction is string from
 * @param address Holds the curreht esi
 * @return Binary form of the instruction
 */
uint32_t movAssembler(char *instruction, uint32_t address) {
  char *savePtr;
  char *destReg = strtok_r(instruction, ",", &savePtr);
  uint32_t destRegIdx = readRegisterIndex(destReg);
  instr = DATAPROC_INSTR + (MOV << OP_START) + (destRegIdx << RD_START);
  readOperand2(savePtr);
  return instr;
}