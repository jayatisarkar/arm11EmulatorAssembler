
#ifndef ARM11_15_UTILS_H
#define ARM11_15_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**
 * Contains all the functions that are common to several instructions
 * so they can be used throughout the program and duplicate code is avoided.
 */

#define INSTR_SIZE 4
#define CPSR 16
#define COND_LENGTH 4
#define FLAG_LENGTH 1
#define REG_LENGTH 4
#define OP_LENGTH 4
#define IMM_LENGTH 8
#define ROTATE_START 8
#define ROTATE_LENGTH 4
#define SHIFT_TYPE_START 5
#define SHIFT_TYPE_LENGTH 2
#define FOURTHBIT 4
#define OPTWO_LENGTH 12
#define PC 15
#define MEMORY_SIZE 65536
#define NO_OF_REGS 17
#define BYTES_PER_INSTR 4
#define N 31
#define Z 30
#define C 29
#define V 28
#define LAST_BIT 31
#define ONE_BIT 1
#define REG_SIZE 32
#define MASK1 0xff
#define MASK2 0xff0000
#define MASK3 0xff00
#define MASK4 0xff000000
#define BYTE3TO0 24
#define BYTE0TO3 24
#define BYTE1TO2 8
#define BYTE2TO1 8
#define REGS_TO_PRINT 13
#define BITS_PER_BYTE 8
#define BIT26 26
#define INSTR_BIT_LEN1 2
#define BIT4 4
#define INSTR_BIT_LEN2 4
#define INSTR_SDT 1
#define INSTR_BRANCH 2
#define INSTR_MULT 9
#define BIT8 8
#define BIT7 7
#define ZERO 0
#define SHIFT_INT_LENGTH 5
#define NUM_OPCODES 23
#define ARRAY_SIZE 1024
#define BASE10 10
#define GPIO20TO29 0x20200008
#define GPIO10TO19 0x20200004
#define GPIO0TO9 0x20200000
#define GPIOCLEAR 0x20200028
#define GPIOSET 0x2020001C

//DATAPROCESSING
#define I_START 25
#define OP_START 21
#define SET_START 20
#define RN_START 16
#define RD_START 12
#define OPTWO_START 0
#define DATAPROC_INSTR 0xE0000000
#define MAX_CONST 0x1F
#define SHIFT_INT 7
#define MAX_EXP 0xFF

//MULTIPLY
#define COND_START 28
#define ACC_START 21
#define DESTREG_START 16
#define REGN_START 12
#define REGS_START 8
#define REGM_START 0
#define MUL_INSTR 0xE0000090
#define ACC 21

//SINGLEDATATRANSFER
#define P_START 24
#define U_START 23
#define L_START 20
#define OFFSET_START 0
#define SDT_OFFSET_LENGTH 12
#define CONDITION 29
#define COND_BITS 3
#define DEST_REG_DISP 12
#define CONSTANT 26
#define MAX_CONSTANT 255
#define PC_OFFSET 8
#define BASE_REG_DISP 16
#define ADD_DISP 23
#define P_DISP 24
#define SHIFT_LEN 3
#define LSR_DISP 5
#define ASR_DISP 6
#define ROR_DISP 5
#define TWO_BIT_SET 2
#define INT_DISP 7
#define REG_S_SHIFT 8
#define REG_S_DISP 4
#define I_DISP 25
#define LOAD_DISP 20

//BRANCH
#define OFFSET_LENGTH 24
#define OFFSET_SHIFT 2
#define MOV 13
#define BRANCH_INSTR 0x0A000000
#define PIPELINE_OFFSET 8

extern uint32_t instr;

struct {
  uint32_t condition, i, opcode, s, rn, rd, opcodetwo;
} dataPInstr;

struct {
  uint32_t condition, accumulate, set, rD, rN, rS, rM;
} multiplyInstr;

struct {
  uint32_t condition, i, p, u, l, rn, rd, offset, rm;
} singleDataTransferInstr;

struct {
  uint32_t condition : COND_LENGTH;
  int32_t offset : OFFSET_LENGTH;
} branchInstruc;

enum shiftType {
  LSL,
  LSR,
  ASR,
  ROR
};

enum condSuffix {
  EQ = 0,
  NE = 1,
  GE = 10,
  LT = 11,
  GT = 12,
  LE = 13,
  AL = 14
};

enum instructionSet {
  DATAPROCESSING,
  MULTIPLY,
  SINGLEDATATRANSFER,
  BRANCH,
  STOP
};

enum status {
  EXECUTE,
  DECODE,
  CLEAR_PIPELINE,
  INVALID_
};

extern uint32_t endValues[ARRAY_SIZE];

extern int numInstructions;

extern uint32_t carryout;

enum instructionSet determineInstrType(uint32_t instruction);

bool getBit(uint32_t x, int bit);

uint32_t generateMask(uint32_t startBit, uint32_t numOfOnes);

uint32_t getBits(uint32_t num, int start, int length);

bool condCheck(uint32_t cond, uint32_t cpsrReg);

uint32_t logicalShiftLeft(uint32_t inst, uint32_t shiftInt);

uint32_t logicalShiftRight(uint32_t inst, uint32_t shiftInt);

uint32_t rotateRight(uint32_t inst, uint32_t shiftInt);

uint32_t arithShiftRight(uint32_t instr, uint32_t shiftInt);

uint32_t updateOpTwo(uint32_t *registers, uint32_t instruction, uint32_t imm);

int notInRange(uint32_t checkNum, uint32_t bound);

uint32_t retrieveWord(uint32_t address, uint8_t *mem);

uint32_t readRegisterIndex(char *token);

void readOperand2(char *instruction);

uint32_t movAssembler(char *instruction, uint32_t address);

bool isGPIO(uint32_t address);

#endif //ARM11_15_UTILS_H