/*  Project 1.1: RISC-V instructions to RISC-V compressed instructions in C89.
    The following is the starter code provided for you. To finish the task, you 
    should define and implement your own functions in translator.c, compression.c, 
    utils.c and their header files.
    Please read the problem description before you start.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "src/compression.h"
#include "src/utils.h"

#include "translator.h"

/*check if file can be correctly opened */
static int open_files(FILE** input, FILE** output, const char* input_name, const char* output_name){ 
    *input = fopen(input_name, "r");
    if (!*input){ /* open input file failed */
        printf("Error: unable to open input file: %s\n", input_name);
        return -1;
    }

    *output = fopen(output_name, "w");
    if (!*output){ /* open output file failed */
        printf("Error: unable to open output file: %s\n", output_name);
        fclose(*input);
        return -1;
    }
    return 0; /* no problem opening files */
}

static int close_files(FILE** input, FILE** output){
    fclose(*input);
    fclose(*output); /* close the files at the end */
    return 0;
}

static void print_usage_and_exit() {
    printf("Usage:\n");
    printf("Run program with translator <input file> <output file>\n"); /* print the correct usage of the program */
    exit(0);
}


/* Run the translator */
int translate(const char* in, const char* out){
    /* allocate the space for input_code and offset_table */
    char input_code[80][33];
    int offset_table[80];
    FILE *input, *output;
    int err = 0, i = 0, j = 0;
    if (in){    /* correct input file name */
        if(open_files(&input, &output, in, out) != 0)
            exit(1);
        /* read in data by line from input */
        myfscanf(input,input_code[i]);
        while (!feof(input)){
            /* if the order can not be compressed */
            if(classify(input_code[i]) <= 0){
                /* initialize the offset_table */
                if(i == 0){
                    offset_table[i] = 4;
                }
                /* get the offset */
                else{
                    offset_table[i] = offset_table[i - 1] + 4;
                }
            }
            /* if the order can be compressed */
            else{
                /* initialize the offset_table */
                if(i == 0){
                    offset_table[i] = 2;
                }
                /* get the offset */
                else{
                    offset_table[i] = offset_table[i - 1] + 2;
                }
            }
            /* store data in in_code, which is 32 bits long */
            myfscanf(input, input_code[++i]);
        }

        for (j = 0; j < i; j++){
            /* compress the order */
            compress(input_code[j], classify(input_code[j]), j, offset_table); 
            /* write correct result to the output file */
            fprintf(output, "%s\n", input_code[j]);
        }
        /* close the file */
        close_files(&input, &output);
    }
    return err;
}

/* main func */
int main(int argc, char **argv){
    char* input_fname, *output_fname;
    int err;
    if (argc != 3) /* need correct arguments */
        print_usage_and_exit();
    /* set fname */
    input_fname = argv[1];
    output_fname = argv[2];
    err = translate(input_fname, output_fname); /* main translation process */
    if (err)
        printf("One or more errors encountered during translation operation.\n"); /* something wrong */
    else
        printf("Translation process completed successfully.\n"); /* correctly output */
    /* exit */
    return 0;
}