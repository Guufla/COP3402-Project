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

int HI = 0;
int LO = 0;

static bool tracing;



static void initialize(){

    tracing = false;

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
                case AND_F: // Bitwise And
                {
                    memory.uwords[GPR.uwords[com.rt] + machine_types_formOffset(com.ot)] = memory.uwords[SP] && (memory.uwords[GPR.uwords[com.rs] + machine_types_formOffset(com.os)]);
                    break;
                }
                case BOR_F: // Bitwise OR
                {
                    memory.uwords[GPR.uwords[com.rt] + machine_types_formOffset(com.ot)] = memory.uwords[SP] || (memory.uwords[GPR.uwords[com.rs] + machine_types_formOffset(com.os)]);
                    break;
                }
                case NOR_F: // Not or
                {   
                    memory.uwords[GPR.uwords[com.rt] + machine_types_formOffset(com.ot)] = !memory.uwords[SP] || (memory.uwords[GPR.uwords[com.rs] + machine_types_formOffset(com.os)]);
                    break;
                }
                case XOR_F: // Exclusive - or
                {
                    memory.uwords[GPR.uwords[com.rt] + machine_types_formOffset(com.ot)] = memory.uwords[SP] ^ (memory.uwords[GPR.uwords[com.rs] + machine_types_formOffset(com.os)]);
                    break;
                }
                case LWR_F: // Load Word into register
                {
                    GPR.uwords[com.rt] = memory.words[GPR.words[com.rs] + machine_types_formOffset(com.os)];
                    break;
                }
                case SWR_F: // Store word in register
                {
                    memory.words[GPR.words[com.rt] + machine_types_formOffset(com.ot)] = GPR.words[com.rs];
                    break;
                }
                case SCA_F: // Store Computed address
                {
                    memory.words[GPR.words[com.rt] + machine_types_formOffset(com.ot)] = GPR.words[com.rs] + machine_types_formOffset(com.os);
                    break;
                }
                case LWI_F: // Load word Indirect
                {
                    memory.words[GPR.words[com.rt] + machine_types_formOffset(com.ot)] = memory.words[memory.words[GPR.words[com.rs] + machine_types_formOffset(com.os)]];
                    break;
                }
                case NEG_F: // Negate
                {
                    memory.words[GPR.words[com.rt] + machine_types_formOffset(com.ot)] = -memory.words[GPR.words[com.rs] + machine_types_formOffset(com.os)];
                    break;
                }

            } 
            break;
        } 
        case other_comp_instr_type:
        {
            other_comp_instr_t ocom = bi.othc;
            switch(ocom.func)
            {
                case LIT_F: // Literal (Load)
                {
                    memory.words[GPR.words[ocom.reg] + machine_types_formOffset(ocom.offset)] = machine_types_sgnExt(ocom.arg);
                    break;
                }
                case ARI_F: // Add register immedidiate
                {
                    GPR.words[ocom.reg] = (GPR.words[ocom.reg] + machine_types_sgnExt(ocom.arg));
                    break;
                }
                case SRI_F: // Subtract register immediate
                {
                    GPR.words[ocom.reg] = (GPR.words[ocom.reg] - machine_types_sgnExt(ocom.arg));
                    break;
                }
                case MUL_F:
                {
                    // MULTIPLICATION FUNCTION
                    break;
                }
                case DIV_F: // Divide (remainder in HI, quotient in LO)
                {
                    HI = memory.words[GPR.words[SP]] % (memory.words[GPR.words[ocom.reg]] + machine_types_sgnExt(ocom.arg));

                    LO = memory.words[GPR.words[SP]] / (memory.words[GPR.words[ocom.reg]] + machine_types_sgnExt(ocom.arg));
                    break;
                }
                case CFHI_F: // Copy from HI
                {
                    memory.words[GPR.words[ocom.reg] + machine_types_formOffset(ocom.offset)] = HI;
                    break;
                }
                case CFLO_F: // Copy from LO
                {
                    memory.words[GPR.words[ocom.reg] + machine_types_formOffset(ocom.offset)] = LO;
                    break;
                }
                case SLL_F: // Shift left logical
                {
                    memory.uwords[GPR.uwords[ocom.reg] + machine_types_formOffset(ocom.offset)] = memory.uwords[GPR.uwords[SP]] << ocom.arg;
                    break;
                }
                case SRL_F: // Shift right logical
                {
                    memory.uwords[GPR.uwords[ocom.reg] + machine_types_formOffset(ocom.offset)] = memory.uwords[GPR.uwords[SP]] >> ocom.arg;
                    break;
                }
                case JMP_F: // Jump
                {
                    PC = memory.uwords[GPR.uwords[ocom.offset] + machine_types_formOffset(ocom.offset)];
                    break;
                }
                case CSI_F: // Call subroutine Indirectly
                {
                    GPR.words[RA] = PC;
                    PC = memory.words[GPR.words[ocom.reg] + machine_types_formOffset(ocom.offset)];
                    break;
                }
                case JREL_F: // Jump Relative Address
                {
                    PC = ((PC - 1) + machine_types_formOffset(ocom.arg));
                    break;
                }
                case SYS_F:
                {
                    
                    switch(instruction_syscall_number(bi))
                    {
                        case exit_sc:
                        {
                            exit(machine_types_sgnExt(bi.othc.offset));
                            break;
                        }
                        case print_str_sc:
                        {
                            memory.words[GPR.words[SP]] = printf("%d" , &memory.words[GPR.words[bi.syscall.reg] + machine_types_formOffset(bi.syscall.offset)]);
                            break;
                        }
                        case print_char_sc:
                        {
                            memory.words[GPR.words[SP]] = fputc(memory.words[GPR.words[bi.syscall.reg] + machine_types_formOffset(bi.syscall.offset)] , stdout);
                            break;
                        }
                        case read_char_sc:
                        {
                            memory.words[GPR.words[bi.syscall.reg] + machine_types_formOffset(bi.syscall.offset)] = getc(stdin);
                            break;
                        }
                        case start_tracing_sc:
                        {
                            tracing = true;
                            break;
                        }
                        case stop_tracing_sc:
                        {
                            tracing = false;
                            break;
                        }

                    }
                    break;
                }

            }
            break;
        } 


        case immed_instr_type:
        {
            immed_instr_t im = bi.immed;
            switch(im.op)
            {
                case ADDI_O: // Add immediate
                {
                    memory.words[GPR.words[im.reg] + machine_types_formOffset(im.offset)] = (memory.words[GPR.words[im.reg]] + machine_types_formOffset(im.offset)) + machine_types_sgnExt(im.immed);
                    break;
                }
                case ANDI_O: // Bitwise and immediate
                {  
                    memory.uwords[GPR.uwords[im.reg] + machine_types_formOffset(im.offset)] = (memory.uwords[GPR.uwords[im.reg]] + machine_types_formOffset(im.offset)) && machine_types_sgnExt(im.immed);
                    break;
                }
                case BORI_O: // Bitwise or immediate
                {
                    memory.uwords[GPR.uwords[im.reg] + machine_types_formOffset(im.offset)] = (memory.uwords[GPR.uwords[im.reg]] + machine_types_formOffset(im.offset)) || machine_types_sgnExt(im.immed);
                    break;
                }
                case NORI_O: // Bitwise or not immediate
                {
                    memory.uwords[GPR.uwords[im.reg] + machine_types_formOffset(im.offset)] = !(memory.uwords[GPR.uwords[im.reg]] + machine_types_formOffset(im.offset)) || machine_types_sgnExt(im.immed);
                    break;
                }
                case XORI_O: // Bitwise Exclusive-Or immediate
                {
                    memory.uwords[GPR.uwords[im.reg] + machine_types_formOffset(im.offset)] = (memory.uwords[GPR.uwords[im.reg]] + machine_types_formOffset(im.offset)) ^ machine_types_sgnExt(im.immed);
                    break;
                }
                case BEQ_O: // Branch on equal
                {
                    if(memory.words[GPR.words[SP]] == memory.words[GPR.words[im.reg] + machine_types_formOffset(im.offset)]){
                        PC = (PC - 1) + machine_types_formOffset(im.immed);
                    }
                    break;
                }
                case BGEZ_O: // Branch >= 0
                {
                    if(memory.words[GPR.words[im.reg] + machine_types_formOffset(im.offset)] >= 0){
                        PC = (PC - 1) + machine_types_formOffset(im.immed);
                    }
                    break;
                }
                case BGTZ_O: // Branch > 0
                {
                    if(memory.words[GPR.words[im.reg] + machine_types_formOffset(im.offset)] > 0){
                        PC = (PC - 1) + machine_types_formOffset(im.immed);
                    }
                    break;
                }
                case BLEZ_O:
                {
                    if(memory.words[GPR.words[im.reg] + machine_types_formOffset(im.offset)] <= 0){
                        PC = (PC - 1) + machine_types_formOffset(im.immed);
                    }
                    break;
                }
                case BLTZ_O:
                {
                    if(memory.words[GPR.words[im.reg] + machine_types_formOffset(im.offset)] < 0){
                        PC = (PC - 1) + machine_types_formOffset(im.immed);
                    }
                    break;
                }
                case BNE_O:
                {
                    if(memory.words[GPR.words[SP]] != memory.words[GPR.words[im.reg] + machine_types_formOffset(im.offset)]){
                        PC = (PC - 1) + machine_types_formOffset(im.immed);
                    }
                    break;
                }
                

            }
            break;
        } 

        case jump_instr_type:
        {
            jump_instr_t jmp = bi.jump;
            switch(jmp.op)
            {
                case JMPA_O:
                {
                    PC = machine_types_formAddress(PC - 1, bi.jump.addr);
                    break;
                }
                case CALL_O:
                {
                    GPR.words[RA] = PC;
                    PC = machine_types_formAddress(PC - 1, bi.jump.addr);
                    break;
                }
                case RTN_O:
                {
                    PC = GPR.words[RA];
                    break;
                }

            }
            break;
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