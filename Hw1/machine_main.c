#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bof.h"
#include "utilities.h"



int main(int argc, char *argv[]) {

    int pFlag = 0;
    argc--;
    argv++;

    for(int i = 1; i < argc; i++){
        if(argc == 2 && (argv[i],"-p") == 0){
            pFlag = 1;
        }
    }

<<<<<<< Updated upstream
=======

    BOFFILE bf = bof_read_open(argv[0]);

    machine_load(bf);

>>>>>>> Stashed changes
    if (pFlag){
        machine_print_loaded_program(stdout);
	    return EXIT_SUCCESS;
    }
    else{
        printf("Flag -p not detected");
    }



    





}




