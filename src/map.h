
#ifndef ARM11_15_MAP_H
#define ARM11_15_MAP_H

#include "utils.h"

typedef struct labelMap {
  char *label;
  uint32_t address;
  struct labelMap *next;
} labelMap;

typedef struct opcodeMap {
  char *opcode;

  uint32_t (*funcPtr)(char *instr, uint32_t address);
} opcodeMap;

#endif //ARM11_15_MAP_H