// COMP1521 20T3 Assignment 1: mips_sim -- a MIPS simulator
// Done by: Harold Song, z5250391
// Final code v1.1 - 31/10/20 - with comments and passed all tests


// PUT YOUR HEADER COMMENT HERE


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "processor.h"
#include "registers.h"

#define MAX_LINE_LENGTH 256
#define INSTRUCTIONS_GROW 64

#define REGISTER_COUNT 32
#define MASK_STD 0x1F
#define MASK_ORI 0x0D
#define SYSCALL_MASK 0x0000000C
#define ADD_MASK 0x00000020
#define MUL_MASK 0x70000002
#define SUB_MASK 0x00000022
#define SLT_MASK 0x0000002A
#define ARITH_MASK_INVERSE 0xFC0007FF







// ADD YOUR #defines HERE
int decode(uint32_t instruction, int trace_mode, int pc);
void execute_instructions(int n_instructions,
                          uint32_t instructions[n_instructions],
                          int trace_mode);
char *process_arguments(int argc, char *argv[], int *trace_mode);
uint32_t *read_instructions(char *filename, int *n_instructions_p);
uint32_t *instructions_realloc(uint32_t *instructions, int n_instructions);


// ADD YOUR FUNCTION PROTOTYPES HERE



// YOU SHOULD NOT NEED TO CHANGE MAIN

int main(int argc, char *argv[]) {
    int trace_mode;
    char *filename = process_arguments(argc, argv, &trace_mode);

    int n_instructions;
    uint32_t *instructions = read_instructions(filename, &n_instructions);

    execute_instructions(n_instructions, instructions, trace_mode);

    free(instructions);
    return 0;
}


// simulate execution of  instruction codes in  instructions array
// output from syscall instruction & any error messages are printed
//
// if trace_mode != 0:
//     information is printed about each instruction as it executed
//
// execution stops if it reaches the end of the array

void execute_instructions(int n_instructions,
                          uint32_t instructions[n_instructions],
                          int trace_mode) {
    // REPLACE CODE BELOW WITH YOUR CODE

    //initialize array of registers with all registers set to 0
    initiateRegisters();

    int pc = 0;
    while (pc < n_instructions) {
      // check for illegal branching
      if (pc < 0){
        printf("Illegal branch to address before instructions: PC = %d\n",pc);
        exit(0);
      }
     if (trace_mode) {
       printf("%d: 0x%08X ", pc, instructions[pc]);
      }
      pc = decode(instructions[pc], trace_mode, pc);
      //pc++; is handled inside decode() function
    }
}



int decode(uint32_t instruction, int trace_mode, int pc){
  //several MASKS were #defined for convenience
  //extract bits to decode instruction
  int s = (instruction >> 21) & MASK_STD;
  int t = (instruction >> 16) & MASK_STD;
  int d = (instruction >> 11) & MASK_STD;
  int16_t immediate = instruction & 0xFFFF;

  //if syscall
  if (instruction == SYSCALL_MASK) syscall(trace_mode);

  //decde the instruction by omparing bits with pre defined masks.
  //call functions from processor.c to process the instructions.
  //add
  else if ((instruction & ARITH_MASK_INVERSE) == ADD_MASK) add(d,s,t,trace_mode);
  //subs
  else if ((instruction & ARITH_MASK_INVERSE) == SUB_MASK) sub(d,s,t,trace_mode);
  //slt
  else if ((instruction & ARITH_MASK_INVERSE) == SLT_MASK) slt(d,s,t,trace_mode);
  //mul
  else if ((instruction & ARITH_MASK_INVERSE) == MUL_MASK) mul(d,s,t,trace_mode);
  //beq
  else if (instruction >> 26 == 4) {
    pc = beq(t,s,immediate,trace_mode, pc);
    return pc; //beq() calculated new Program Counter value, so we can go ahead and return it to execute_instructions
  }
  //bne
  else if (instruction >> 26 ==5) {
    pc = bne(t,s,immediate,trace_mode,pc);
    return pc; //bne() calculated new Program Counter value, so we can go ahead and return it to execute_instructions
  }
  //addi
  else if (instruction >> 26 == 8) addi(t,s,immediate,trace_mode);
  //if ori
  else if (instruction >> 26 == 0x0D) ori(t,s,immediate,trace_mode);
  //if lui
  else if (instruction >> 21 == 0x1E0) lui(t,immediate, trace_mode);
  //if no instruction matched, invalid code.
  else{
    printf("invalid instruction code\n");
    exit(0);
  }
  // increase program counter after instruction is processed.
  pc++;
  return pc;
}

// ADD YOUR FUNCTIONS HERE



// YOU DO NOT NEED TO CHANGE CODE BELOW HERE


// check_arguments is given command-line arguments
// it sets *trace_mode to 0 if -r is specified
//          *trace_mode is set to 1 otherwise
// the filename specified in command-line arguments is returned

char *process_arguments(int argc, char *argv[], int *trace_mode) {
    if (
        argc < 2 ||
        argc > 3 ||
        (argc == 2 && strcmp(argv[1], "-r") == 0) ||
        (argc == 3 && strcmp(argv[1], "-r") != 0)) {
        fprintf(stderr, "Usage: %s [-r] <file>\n", argv[0]);
        exit(1);
    }
    *trace_mode = (argc == 2);
    return argv[argc - 1];
}


// read hexadecimal numbers from filename one per line
// numbers are return in a malloc'ed array
// *n_instructions is set to size of the array

uint32_t *read_instructions(char *filename, int *n_instructions_p) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "%s: '%s'\n", strerror(errno), filename);
        exit(1);
    }

    uint32_t *instructions = NULL;
    int n_instructions = 0;
    char line[MAX_LINE_LENGTH + 1];
    while (fgets(line, sizeof line, f) != NULL) {

        // grow instructions array in steps of INSTRUCTIONS_GROW elements
        if (n_instructions % INSTRUCTIONS_GROW == 0) {
            instructions = instructions_realloc(instructions, n_instructions + INSTRUCTIONS_GROW);
        }

        char *endptr;
        instructions[n_instructions] = strtol(line, &endptr, 16);
        if (*endptr != '\n' && *endptr != '\r' && *endptr != '\0') {
            fprintf(stderr, "%s:line %d: invalid hexadecimal number: %s",
                    filename, n_instructions + 1, line);
            exit(1);
        }
        n_instructions++;
    }
    fclose(f);
    *n_instructions_p = n_instructions;
    // shrink instructions array to correct size
    instructions = instructions_realloc(instructions, n_instructions);
    return instructions;
}


// instructions_realloc is wrapper for realloc
// it calls realloc to grow/shrink the instructions array
// to the speicfied size
// it exits if realloc fails
// otherwise it returns the new instructions array
uint32_t *instructions_realloc(uint32_t *instructions, int n_instructions) {
    instructions = realloc(instructions, n_instructions * sizeof *instructions);
    if (instructions == NULL) {
        fprintf(stderr, "out of memory");
        exit(1);
    }
    return instructions;
}
