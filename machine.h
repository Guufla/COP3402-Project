
#ifndef _MACHINE_H

#define _MACHINE_H
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "regname.h"
#include "machine_types.h"
#include "bof.h"
#include "instruction.h"

// Define external variables so they can be used across files
extern union gpr_u {
    word_type words[NUM_REGISTERS];
    uword_type uwords[NUM_REGISTERS];
} GPR;

extern address_type PC;

extern int HI;
extern int LO;

extern bool tracing;

// Function declarations
void initialize();
void machine_execute_instr(bin_instr_t bi);

#define MAX_STACK_HEIGHT 2048

#endif

