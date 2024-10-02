
#ifndef _MACHINE_H

#define _MACHINE_H
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "regname.h"
#include "machine_types.h"
#include "bof.h"
#include "instruction.h"
#define MEMORY_SIZE_IN_WORDS 32768

// Define external variables so they can be used across files
union gpr_u {
    word_type words[NUM_REGISTERS];
    uword_type uwords[NUM_REGISTERS];
};

union mem_u {
    word_type words[MEMORY_SIZE_IN_WORDS];
    uword_type uwords[MEMORY_SIZE_IN_WORDS];
    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
};

extern union gpr_u GPR;
extern union mem_u memory;
extern address_type PC;

extern int HI;
extern int LO;

extern bool tracing;

// Function declarations
void initialize();
void machine_execute_instr(bin_instr_t bi);

#define MAX_STACK_HEIGHT 2048

#endif