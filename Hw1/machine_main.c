#include "machine.c"

// a size for the memory ( 2 ^ 1 6 words = 32K words )
#define MEMORY_SIZE_IN_WORDS 32768
static union mem_u {
    word_type words[MEMORY_SIZE_IN_WORDS];
    uword_type uwords[MEMORY_SIZE_IN_WORDS];
    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
} memory;

int main(void) {

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



    





}




