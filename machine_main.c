#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "instruction.h"
#include "bof.h"
#include "machine_types.h"
#include "regname.h"
#include "utilities.h"
#include "machine.h"
#include "assert.h"

// a size for the memory ( 2 ^ 16 words = 32K words )
#define MEMORY_SIZE_IN_WORDS 32768
static union mem_u {
    word_type words[MEMORY_SIZE_IN_WORDS];
    uword_type uwords[MEMORY_SIZE_IN_WORDS];
    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
} memory;

int main(int argc, char *argv[]) {

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



    int pFlag = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            pFlag = 1;
            BOFFILE bof = bof_read_open(argv[i+1]);
            BOFHeader bofHeader = bof_read_header(bof);
            bin_instr_t instru;

            //printf(".text\t%u\n", bofHeader.text_start_address);
            printf("Address Instruction\n");
            for (int j = 0; j < bofHeader.text_length; j++) {
                instru = instruction_read(bof);
                printf("%d:\t%s\n", j, instruction_assembly_form(j, instru));
            }
            word_type data = bofHeader.data_start_address;
            if(bofHeader.data_length == 0) //no data case
            {
              printf("%u: 0   ",data);
                  printf("...\n");
            }
            else{
            word_type count = 0;
            word_type repeat = 0;
            for (int k = 0; k < bofHeader.data_length; k++) {
                word_type dataItem = bof_read_word(bof);
                if(dataItem == 0) repeat++;
                if(repeat == 2)//repeat location case
                {
                  printf("...\n");
                  break;
                }
                
                printf("%u: %d   ",data, dataItem); //print data
                data++;
                count++;
                
                if(count != 0 && count % 5 == 0) printf("\n"); //print new line every 5 data.
               
                

                if(k == bofHeader.data_length - 1)
                {
                  printf("%u: 0   ",data);
                  
                  if(k == 3){printf("\n");} //edge case
                  
                  printf("...\n");
                } 
            }
            }
            //printf("\n");
            bof_close(bof);
            return 0;
        }
    }

    if (strcmp(argv[1], "-p") != 0) {
        BOFFILE bof = bof_read_open(argv[1]);
        BOFHeader bofHeader = bof_read_header(bof);
        bin_instr_t instruction;
        // initialize all registers based on the header from the bof
        gp = bofHeader.data_start_address;
        fp = bofHeader.stack_bottom_addr;
        sp = bofHeader.stack_bottom_addr;
        pc = bofHeader.text_start_address;
        // Loads instructions into memory from memory.instrs[beginning_address] to memory.instrs[end_address]
        for (int j = bofHeader.text_start_address; j < bofHeader.text_length; j++) {
            memory.instrs[j] = instruction_read(bof);
        }

          // Loads data into memory from memory.words[beginning_address] to memory.words[end_address]
        // accessed from $gp, distinct from $pc section
        for (int k = bofHeader.data_start_address; k < bofHeader.data_length; k++) {
            memory.words[k] = bof_read_word(bof);
        }

        // Process instructions so we can use data from memory (moves stack downwards).
        while (pc < fp) {
            instruction = memory.instrs[pc];
            pc++; // pc is supposed to be incremented before instruction is executed according to 3.3 in SSM
            machine_execute_instr(instruction);
        }
        bof_close(bof);
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