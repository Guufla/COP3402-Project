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

// a size for the memory (2 ^ 16 words = 32K words)
#define MEMORY_SIZE_IN_WORDS 32768
//union mem_u {
//    word_type words[MEMORY_SIZE_IN_WORDS];
//    uword_type uwords[MEMORY_SIZE_IN_WORDS];
//    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
//} memory;

int main(int argc, char *argv[]) {

    int pFlag = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0) {
            pFlag = 1;
            BOFFILE bof = bof_read_open(argv[i + 1]);
            BOFHeader bofHeader = bof_read_header(bof);
            bin_instr_t instru;

            printf("Address Instruction\n");
            for (int j = 0; j < bofHeader.text_length; j++) {
                instru = instruction_read(bof);
                printf("%d:\t%s\n", j, instruction_assembly_form(j, instru));
            }

            word_type data = bofHeader.data_start_address;
            if (bofHeader.data_length == 0) {
                printf("%u: 0   ", data);
                printf("...\n");
            } else {
                word_type count = 0;
                word_type repeat = 0;
                for (int k = 0; k < bofHeader.data_length; k++) {
                    word_type dataItem = bof_read_word(bof);
                    if (dataItem == 0) repeat++;
                    if (repeat == 2) {
                        printf("...\n");
                        break;
                    }

                    printf("%u: %d   ", data, dataItem); // print data
                    data++;
                    count++;

                    if (count != 0 && count % 5 == 0) printf("\n"); // print new line every 5 data

                    if (k == bofHeader.data_length - 1) {
                        printf("%u: 0   ", data);
                        if (k == 3) { printf("\n"); } // edge case
                        printf("...\n");
                    }
                }
            }
            bof_close(bof);
            return 0;
        }
    }

    if (strcmp(argv[1], "-p") != 0) {
        BOFFILE bof = bof_read_open(argv[1]);
        BOFHeader bofHeader = bof_read_header(bof);
        bin_instr_t instruction;

        // Initialize all registers using the GPR array in machine.c
        GPR.words[GP] = bofHeader.data_start_address;
        GPR.words[FP] = bofHeader.stack_bottom_addr;
        GPR.words[SP] = bofHeader.stack_bottom_addr;
        PC = bofHeader.text_start_address;

        // Load instructions into memory
        for (int j = bofHeader.text_start_address; j < bofHeader.text_length; j++) {
            memory.instrs[j] = instruction_read(bof);
        }

        // Load data into memory
        for (int k = bofHeader.data_start_address; k < bofHeader.data_start_address + bofHeader.data_length; k++) {
            memory.words[k] = bof_read_word(bof);
        }

        // Process instructions
        while (PC < bofHeader.text_start_address + bofHeader.text_length) {
            instruction = memory.instrs[PC];

            // Print the PC and Registers
            printf("      PC: %d\n", PC);
            printf("GPR[$gp]: %d \tGPR[$sp]: %d \tGPR[$fp]: %d \tGPR[$r3]: %d \tGPR[$r4]: %d\n", 
                GPR.words[GP], GPR.words[SP], GPR.words[FP], GPR.words[3], GPR.words[4]);
            printf("GPR[$r5]: %d \tGPR[$r6]: %d \tGPR[$ra]: %d\n", 
                GPR.words[5], GPR.words[6], GPR.words[RA]);

            // Print data items
            word_type count = 0;
            word_type repeat = 0;
            word_type data_addr = bofHeader.data_start_address;
            for (int k = bofHeader.data_start_address; k <= bofHeader.stack_bottom_addr; k++) {
                word_type dataItem = memory.words[k];
        	if (tracing == false) {
                printf("Skipping iteration due to tracing being false\n");
            	continue;
        	}

		if (k == bofHeader.stack_bottom_addr) {
 		    printf("\n%u: %d  ", data_addr, dataItem);
		    break; 
		}
		//printf(" sp: %u: %d",GPR.words[SP], memory.words[GPR.words[SP]]);
                if (dataItem == 0) repeat++;
		else { 
		    repeat = 0;
		    //printed_dots = false;
		}


                if (repeat == 2) {
                    printf("...   ");
		    data_addr++;
		    continue;
		    
                }

		if (repeat > 2) {
		    data_addr++;
		    continue;
		}

		if (repeat < 2) {
                    printf("%u: %d   ", data_addr, dataItem);
		    count++;// print data
		}

                data_addr++;
                //count++;

                if (count != 0 && count % 5 == 0) printf("\n"); // print new line every 5 data

                //if (k == bofHeader.stack_bottom_addr - 1) {
                //    printf("%u: %d   ", data_addr, dataItem);
		//    printf("...\n");
                    //if (k == 3) { printf("\n"); } // edge case
                    //printf("...\n");
                
            }

            printf("\n==>\t  %d: %s\n", PC, instruction_assembly_form(PC, memory.instrs[PC]));
            PC++; // increment PC before execution
            machine_execute_instr(instruction);
        }
        bof_close(bof);
    }
}
