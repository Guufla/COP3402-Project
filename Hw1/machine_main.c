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
    int gp = 0;
    int sp = 0;
    int fp = 0;
    int r3 = 0;
    int r4 = 0;
    int r5 = 0;
    int r6 = 0;
    int ra = 0;

    int pc = 0;
    int hi = 0;
    int lo = 0;



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

           for (int j = 1; j < bofHeader.text_length; j++) {
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