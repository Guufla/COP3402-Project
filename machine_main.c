#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "instruction.h"
#include "bof.h"
#include "machine_types.h"
#include "regname.h"
#include "utilities.h"
#include "machine.c"
#include "assert.h"

// a size for the memory ( 2 ^ 1 6 words = 32K words )
#define MEMORY_SIZE_IN_WORDS 32768
static union mem_u {
    word_type words[MEMORY_SIZE_IN_WORDS];
    uword_type uwords[MEMORY_SIZE_IN_WORDS];
    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
} memory;

int main(int argc, char *argv[]) {

    union mem_u memory;
    // Represents the Registers
    word_type gp = 0;
    word_type sp = 0;
    word_type fp = 0;
    word_type r3 = 0;
    word_type r4 = 0;
    word_type r5 = 0;
    word_type r6 = 0;
    word_type ra = 0;

    word_type pc = 0;
    word_type hi = 0;
    word_type lo = 0;



    int pFlag = 0;
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-p") == 0){
            pFlag = 1;
            printf("P flag is checked in!\n");
            printf("%s", argv[i+1]);
            BOFFILE bof = bof_read_open(argv[i+1]);
            BOFHeader bofHeader = bof_read_header(bof);
            bin_instr_t instru;

            printf("File and file header are read in!\n");
            printf(".text\t%u\n", bofHeader.text_start_address);

           for (int j = 0; j < bofHeader.text_length; j++) {
                instru = instruction_read(bof);
                printf("a%d:\t%s\n", j, instruction_assembly_form(j, instru));
                instruction_assembly_form(j, memory.instrs[j]);
                printf("debug: Instruction printed!\n");
           }
           for (int k = 0; k < bofHeader.data_length; k++) {
                word_type dataItem = bof_read_word(bof);
                printf("debug: Data word read!\n");
           }
        }
    }


    if (pFlag){
        printf("Flag -p detected");
    }
    else{
        printf("Flag -p not detected");
        BOFFILE bof = bof_read_open(argv[1]);
        BOFHeader bofHeader = bof_read_header(bof);
        bin_instr_t instruction;
        // initialize all registers based on the header from the bof
        gp = bofHeader.data_start_address;
        fp = bofHeader.stack_bottom_addr;
        sp = bofHeader.stack_bottom_addr;
        pc = bofHeader.text_start_address;
        // Loads instructions into memory from memory.instrs[beginning_address] to memory.instrs[end_address]
        for (int j = 0; j < bofHeader.text_length; j++) {
            memory.instrs[j] = instruction_read(bof);
        }

        // Process instructions so we can use data from memory!
        while (pc < bofHeader.text_length) {
            instruction = memory.instrs[pc];
            // Execute the instruction in a loop.
            for (int i = 0; i < bofHeader.text_length; i++) {
                machine_execute_instr(memory.instrs[pc]);  // TODO

            }

        }
        
        
        // Loads data into memory from memory.words[beginning_address] to memory.words[end_address]
        // THIS WILL OVERWRITE MEMORY IN THE INSTRUCTION MEMORY SECTION
        for (int k = 0; k < bofHeader.data_length; k++) {
            memory.words[k] = bof_read_word(bof);

        }

}


/*
// read in binary instruction, offset by size of instruction
                    bin_instr_t instr = instruction_read(bof);
                    word_type type = bof_read_word(bof);
                    printf("wordType: %d" , type);
                    bofaddr += sizeof(instr);
                    printf("Address: %d \n" , bofaddr);
                    // print assembly form of instruction
                    printf(instruction_assembly_form(bofaddr, instr));

*/