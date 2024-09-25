#include "machine_types.h"
#include "machine_types.c"
#include "instruction.c"
#include "instruction.h"
#include "bof.c"
#include "bof.h"
#include "regname.c"
#include "regname.h"
#include "utilities.c"
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
