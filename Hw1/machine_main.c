#include <stdio.h>
#include <string.h>

#include "machine.c"

// a size for the memory ( 2 ^ 1 6 words = 32K words )
#define MEMORY_SIZE_IN_WORDS 32768
static union mem_u {
    word_type words[MEMORY_SIZE_IN_WORDS];
    uword_type uwords[MEMORY_SIZE_IN_WORDS];
    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
} memory;

int main(int argc, char *argv[]) {


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
            // binary object file offset in bytes
            int bofaddr = 0;
            // populates bof BOFFILE type with information, opens file
             BOFFILE bof = bof_read_open(argv[i+1]);
             // while bof is not at the end, read instructions to assembler
             while (!feof(bof.fileptr)) {
                // read in binary instruction, offset by size of instruction
                 bin_instr_t instr = instruction_read(bof);
                 bofaddr += sizeof(instr);
                 // print assembly form of instruction
                 printf(instruction_assembly_form(bofaddr, instr));
             }
        }
    }


    if (pFlag){
        printf("Flag -p detected");
    }
    else{
        printf("Flag -p not detected");
    }





