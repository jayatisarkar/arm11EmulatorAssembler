
#ifndef ARM11_15_BRANCH_H
#define ARM11_15_BRANCH_H

#include "utils.h"

/**
 * Handles the branch instruction of the assembler.
 */

bool isLabel(char *line);

void fillLabelMap(char *fileName);

void freeLabels(void);

void freeLabelMap(void);

uint32_t generalBranchAssembler(char *instruction, uint32_t address,
                                enum condSuffix cond);

uint32_t beqAssembler(char *expression, uint32_t address);

uint32_t bneAssembler(char *expression, uint32_t address);

uint32_t bgeAssembler(char *expression, uint32_t address);

uint32_t bltAssembler(char *expression, uint32_t address);

uint32_t bgtAssembler(char *expression, uint32_t address);

uint32_t bleAssembler(char *expression, uint32_t address);

uint32_t bAssembler(char *expression, uint32_t address);

#endif //ARM11_15_BRANCH_H
