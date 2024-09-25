#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>
#include "machine_types.h"
#include "machine.h"
#include "regname.h"
#include "utilities.h"

// a size for the memory ( 2 ^ 1 6 words = 32K words )
#define MEMORY_SIZE_IN_WORDS 32768


static union mem_u {
    word_type words[MEMORY_SIZE_IN_WORDS];
    uword_type uwords[MEMORY_SIZE_IN_WORDS];
    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
} memory;

static union gpr_u{
    word_type ints[NUM_REGISTERS];
    uword_type floats[NUM_REGISTERS];
}GPR;

static address_type PC;



static void initialize(){
    for(int j = 0; j < NUM_REGISTERS; j++){
        GPR.ints[j] = 0;
    }
    \
    for(int j = 0; j < MEMORY_SIZE_IN_WORDS; j++){
        memory.words[j] = 0;
    }
}

void machine_load(BOFFILE bf){

}

void machine_execute_instr(bin_instr_t bi){

}

static void print_global_data(FILE *out){

}

static void print_registers(FILE *out){

}

static void print_runtime_stack_AR(FILE *out){
    
}

void machine_print_state(FILE *out){
    print_registers(out);
    print_global_data(out);
    print_runtime_stack_AR(out);
}