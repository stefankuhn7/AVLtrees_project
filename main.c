#include <stdio.h>
#include <stdlib.h>
#include "evaluate_tree.h"
#include "build_tree.h"

int main(int argc, char ** argv){
    if (argv[1][1] == 'b'){
        if (argc != 4) fprintf(stderr, "Invalid Input.");
        
        // input is valid, so need to build the tree based off input
        if(Build_tree(argv[2], argv[3]) == -1){
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }
        fprintf(stdout, "%d\n", 1);
        return EXIT_SUCCESS;
    }
    else if (argv[1][1] == 'e'){
        if (argc != 3) fprintf(stderr, "Invalid Input.");
        int valid_in = 1, bst = 1, bal = 1;
        Eval_main(argv[2], &valid_in, &bst, &bal);
        if(valid_in == 0){
            fprintf(stdout, "0, 0, 0");
            return EXIT_FAILURE;
        }
        else if(valid_in == -1){
            fprintf(stdout, "%d\n", valid_in);
        }
        int num_print = fprintf(stdout, "%d, %d, %d\n", valid_in, bst, bal);
        if (num_print == 3){
            return EXIT_SUCCESS;
        }
        else{
            return EXIT_FAILURE;
        }
        
    }
}

