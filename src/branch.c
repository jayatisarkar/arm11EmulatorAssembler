
#include "branch.h"
#include "map.h"

labelMap *headNode = NULL;
int numNodes = 0;
int numLabels = 0;

/**
 * Checks if the given line of the file is a label by parsing the string and
 * checking if the last character is a colon
 * @param line Character pointer to a line in the file
 * @return True if it is a label, false otherwise
 */
bool isLabel(char *line) {
  size_t lengthOfLine = strlen(line);
  char *split = strtok(line, ":");
  size_t lengthOfLineAfterSplit = strlen(split);
  //length remains the same if the colon is not present
  return lengthOfLine != lengthOfLineAfterSplit;
}

/**
 * Allocates space for a label in the memory and returns a copy of the label
 * in the form of a character pointer
 * @param labelName Name of the label
 * @return A copy of the label
 */
char *makeLabel(char *labelName) {
  char *copyLabel = calloc(strlen(labelName) + 1, sizeof(char));
  if (copyLabel == NULL) {
    perror("makeLabel");
    exit(EXIT_FAILURE);
  }
  strcpy(copyLabel, labelName);
  return copyLabel;

}

/**
 * Reads the file and creates a symbol table used for associating labels with
 * memory addresses, by implementing a linked list using a struct which contains
 * a character pointer to a label, a 32 bit address of the label name, and a
 * pointer to the next struct
 * @param fileName Name of the file to be read from
 */
void fillLabelMap(char *fileName) {
  FILE *fp;
  fp = fopen(fileName, "r");
  char *line = NULL;
  size_t len = 0;
  uint32_t address = 0;
  labelMap *newNode = NULL;
  labelMap *prevNode = NULL;
  char *split;
  if (fp == NULL) {
    perror("readFile");
    exit(EXIT_FAILURE);
  }

  //reads the file line by line
  while (getline(&line, &len, fp) != -1) {

    // skips blank lines
    if (line[0] == '\n') {
      continue;
    }
    if (isLabel(line)) {
      numLabels++;
      split = strtok(line, ":");
      // allocate memory for the node
      newNode = malloc(sizeof(labelMap));

      if (newNode == NULL) {
        perror("fillLabelMap");
        exit(EXIT_FAILURE);
      }

      memset(newNode, 0, sizeof(labelMap));

      // 1st time called - initialize headNode
      if (headNode == NULL) {
        headNode = newNode;
      }
      // populate values
      newNode->label = makeLabel(split);
      newNode->address = address;

      // insert the node into the linked list
      if (prevNode != NULL) {
        prevNode->next = newNode;
      }
      numNodes++;
      prevNode = newNode;

    } else {
      address += BYTES_PER_INSTR;
      numInstructions++;
    }
  }

  fclose(fp);
  free(line);

}

/**
 * Frees the space allocated in the memory for the label
 */
void freeLabels(void) {
  labelMap *lMap = headNode;
  for (int i = 0; i < numLabels; i++) {
    free(lMap->label);
    lMap = lMap->next;
  }
}

/**
 * Frees the space allocated in the memory for the label symbol table
 */
void freeLabelMap(void) {
  labelMap *lMap = headNode;
  labelMap *nextlMap;
  while (lMap != NULL) {
    nextlMap = lMap->next;
    free(lMap);
    lMap = nextlMap;
  }
}

/**
 * Returns the address of the given label in the memory
 * @param label The name of the label
 * @return Address of the label in the form of a 32  bit integer
 */
uint32_t lookupLabel(char *label) {
  labelMap *copyHeadNode = headNode;
  char *labelName = strtok(label, " \t\n");
  while (strcmp(copyHeadNode->label, labelName) != 0) {
    copyHeadNode = copyHeadNode->next;
  }
  return copyHeadNode->address;
}

/**
 * Generates the binary format of the given instruction by parsing the string
 * and calculating the offset between the current address and the label address
 * @param instruction The current instruction
 * @param address Keeps track of the current address of the instruction
 * @param cond Determines which branch instruction is to be executed
 * @return Binary format of the given branch instruction
 */
uint32_t generalBranchAssembler(char *instruction, uint32_t address,
                                enum condSuffix cond) {
  instr = (uint32_t) (BRANCH_INSTR + (cond << COND_START));
  int32_t offset;
  //splits the string to get the second operand if its a constant or register
  if (*instruction == '#') {
    instruction++;
    //checks for hexadecimal
    if (*(instruction + 1) == 'x') {
      offset = (int32_t) strtol(instruction, NULL, 0) - address;
    } else {
      offset = (int32_t) strtol(instruction, NULL, BASE10) - address;
    }
  } else {
    offset = lookupLabel(instruction) - address;
  }
  offset -= PIPELINE_OFFSET;
  offset >>= OFFSET_SHIFT;
  offset = getBits((uint32_t) offset, 0, OFFSET_LENGTH);
  instr += offset;
  return instr;
}

/**
 * Generates the binary format of the instruction according to the
 * given instruction
 * @param expression  The expression/target address of the given instruction
 * @param address The current address of the instruction
 * @return Binary form of the given instruction
 */

uint32_t beqAssembler(char *expression, uint32_t address) {
  return generalBranchAssembler(expression, address, EQ);
}

uint32_t bneAssembler(char *expression, uint32_t address) {
  return generalBranchAssembler(expression, address, NE);
}

uint32_t bgeAssembler(char *expression, uint32_t address) {
  return generalBranchAssembler(expression, address, GE);
}

uint32_t bltAssembler(char *expression, uint32_t address) {
  return generalBranchAssembler(expression, address, LT);
}

uint32_t bgtAssembler(char *expression, uint32_t address) {
  return generalBranchAssembler(expression, address, GT);
}

uint32_t bleAssembler(char *expression, uint32_t address) {
  return generalBranchAssembler(expression, address, LE);
}

uint32_t bAssembler(char *expression, uint32_t address) {
  return generalBranchAssembler(expression, address, AL);
}