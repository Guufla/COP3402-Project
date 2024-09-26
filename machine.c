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
    word_type words[NUM_REGISTERS];
    uword_type uwords[NUM_REGISTERS];
}GPR;

static address_type PC;



static void initialize(){
    for(int j = 0; j < NUM_REGISTERS; j++){
        GPR.words[j] = 0;
        GPR.uwords[j] = 0;
    }
    \
    for(int j = 0; j < MEMORY_SIZE_IN_WORDS; j++){
        memory.words[j] = 0;
        memory.uwords[j] = 0;
    }
}

void machine_load(BOFFILE bf){

}

void machine_execute_instr(bin_instr_t bi){
    PC = PC + BYTES_PER_WORD;
    
    instr_type it = instruction_type(bi);
    switch(it){
        case comp_instr_type:
        {
            comp_instr_t com = bi.comp;
            switch(com.func)
            {
                case NOP_F:
                {
                    // Does literally nothing
                    break;
                }
                case ADD_F: // Add
                {
                    memory.words[GPR.words[com.rt] + machine_types_formOffset(com.ot)] = memory.words[GPR.words[SP]] + machine_types_formOffset(com.os);
                    break;
                }
                case SUB_F: // Subtract
                {
                    memory.words[GPR.words[com.rt] + machine_types_formOffset(com.ot)] = memory.words[GPR.words[SP]] - machine_types_formOffset(com.os);
                    break;
                }
                case CPW_F: // Copy word
                {
                    memory.words[GPR.words[com.rt] + machine_types_formOffset(com.ot)] = memory.words[GPR.words[com.rs]] + machine_types_formOffset(com.os);
                    break;
                }
                case BOR_F: // Bitwise And
                {
                    memory.uwords[GPR.uwords[com.rt] + machine_types_formOffset(com.ot)] = memory.uwords[SP] && (memory.uwords[GPR.uwords[com.rs] + machine_types_formOffset(com.os)]);
                    break;
                }
                case NOR_F:
                {
                    break;
                }
                case XOR_F:
                {
                    break;
                }
                case LWR_F:
                {
                    break;
                }
                case SWR_F:
                {
                    break;
                }
                case SCA_F:
                {
                    break;
                }
                case LWI_F:
                {
                    break;
                }
                case NEG_F:
                {
                    break;
                }

            }
        }
        case other_comp_instr_type:
        {
            other_comp_instr_t ocom = bi.othc;
            switch(ocom.func)
            {
                case LIT_F:
                {
                    break;
                }
                case ARI_F:
                {
                    break;
                }
                case SRI_F:
                {
                    break;
                }
                case MUL_F:
                {
                    break;
                }
                case DIV_F:
                {
                    break;
                }
                case CFHI_F:
                {
                    break;
                }
                case CFLO_F:
                {
                    break;
                }
                case SLL_F:
                {
                    break;
                }
                case SRL_F:
                {
                    break;
                }
                case JMP_F:
                {
                    break;
                }
                case CSI_F:
                {
                    break;
                }
                case JREL_F:
                {
                    break;
                }
                case SYS_F:
                {
                    // Something to do with the system table
                    break;
                }

            }
        }
        case syscall_instr_type:
        {
            switch(instruction_syscall_number(bi))
            {
                case exit_sc:
                {
                    break;
                }
                case print_str_sc:
                {
                    break;
                }
                case print_char_sc:
                {
                    break;
                }
                case read_char_sc:
                {
                    break;
                }
                case start_tracing_sc:
                {
                    break;
                }
                case stop_tracing_sc:
                {
                    break;
                }
                

            }
        }
        case immed_instr_type:
        {
            immed_instr_t im = bi.immed;
            switch(im.op)
            {
                case ADDI_O:
                {
                    break;
                }
                case ANDI_O:
                {
                    break;
                }
                case BORI_O:
                {
                    break;
                }
                case NORI_O:
                {
                    break;
                }
                case XORI_O:
                {
                    break;
                }
                case BEQ_O:
                {
                    break;
                }
                case BGEZ_O:
                {
                    break;
                }
                case BGTZ_O:
                {
                    break;
                }
                case BLEZ_O:
                {
                    break;
                }
                case BLTZ_O:
                {
                    break;
                }
                case BNE_O:
                {
                    break;
                }
                case JMPA_O:
                {
                    break;
                }
                case CALL_O:
                {
                    break;
                }
                case RTN_O:
                {
                    break;
                }
                

            }
        }
        // DO NOT FORGET BREAKS AT THE END OF EACH CASE !!!!!
    }
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