#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "processor.h"
#include "registers.h"

/*
All functions here take in trace_mode as input.
If trace_mode == true then functions run in verbose mode as per assignment specs.
if false then functions would process the instructions and update registers but do not print anything (except syscall)
*/

void syscall(int trace_mode){
  if (trace_mode){
    printf ("syscall");
    printf("\n>>> syscall %d\n", registers[2]);

    if (registers[2] == 1) printf("<<< %d\n", registers[4]); // if $v0 == 1, print int
    else if (registers[2] == 10) exit(0);// if $v0 ==10, exit program
    else if (registers[2] == 11) printf("<<< %c\n", registers[4]); // if $v0 == 11, print char
    else{
      printf("Unknown system call: %d\n", registers[2]);//if $v0 unknown / invalid, exit
      exit(0);
    }
  }
  //else if trace_mode is not enabled
  else{
    if (registers[2] == 1) printf("%d", registers[4]);// if $v0 == 1, print int
    else if (registers[2] == 10) exit(0);// if $v0 ==10, exit program
    else if (registers[2] == 11) printf("%c", registers[4]);// if $v0 == 11, print char
    else{
      printf("Unknown system call: %d\n", registers[2]); //if $v0 unknown / invalid, exit
      exit(0);
    }
  }
}

//        *arithmetic functions*
//arithmetic functions hould be self-explanatory.
//all functions do not update register $t0
void add(int d, int s, int t, int trace_mode){
  if (trace_mode){
    printf("add  $%d, $%d, $%d\n", d, s, t);
    printf(">>> $%d = %d\n", d, (registers[s] + registers[t]));
  }
  if (d != 0) registers[d] = registers[s] + registers[t];
}

void sub(int d, int s, int t, int trace_mode){
  if (trace_mode){
    printf("sub  $%d, $%d, $%d\n", d, s, t);
    printf(">>> $%d = %d\n", d, (registers[s] - registers[t]));
  }
  if (d != 0) registers[d] = registers[s] - registers[t];

}

void slt(int d, int s, int t, int trace_mode){
  if (trace_mode){
    printf("slt  $%d, $%d, $%d\n", d, s, t);
    printf(">>> $%d = %d\n", d, (registers[s] < registers[t]));
  }
  if (d != 0) registers[d] = (registers[s] < registers[t]);
}

void mul(int d, int s, int t, int trace_mode){
  if (trace_mode) {
    printf("mul  $%d, $%d, $%d\n", d, s, t);
    printf(">>> $%d = %d\n", d, (registers[s] * registers[t]));
  }
  if (d != 0) registers[d] = registers[s] * registers[t];
}

//                        *branching instructions*
//if conditions are met, calculates updated Program Counter(PC) and return to decode() which returns it to main()
int beq(int t, int s,int16_t immediate, int trace_mode, int pc){
  if (trace_mode){
    printf("beq  $%d, $%d, %d\n", s, t, immediate);
  }
  if (registers[s] == registers[t]) { //beq
    pc += immediate;
    if (trace_mode) printf(">>> branch taken to PC = %d\n", pc);
    return pc;
  }
  else if (trace_mode) printf(">>> branch not taken\n");
  pc++;
  return pc;
}

//if conditions are met, calculates updated Program Counter(PC) and return to decode() which returns it to main()
int bne(int t, int s,int16_t immediate, int trace_mode, int pc){
  if (trace_mode){
    printf("bne  $%d, $%d, %d\n", s, t, immediate);
  }
  if (registers[s] != registers[t]){ //bne
    pc += immediate;
    if (trace_mode) printf(">>> branch taken to PC = %d\n", pc);
    return pc;
  }
  else if (trace_mode) printf(">>> branch not taken\n");
  pc++;
  return pc;
}

//        *arithmetic / bit manipulation functions*
//functions hould be self-explanatory.
//all functions do not update register $t0
void addi(int t, int s,int16_t immediate, int trace_mode){
  if (trace_mode){
    printf("addi $%d, $%d, %d\n", t, s, immediate);
    printf(">>> $%d = %d\n", t, (registers[s] + immediate));
  }
  if (t != 0) registers[t] = (registers[s] + immediate);
}

void lui(int t, int16_t immediate, int trace_mode){
  if (t != 0) registers[t] = immediate << 16;
  if (trace_mode){
    printf("lui  $%d, %d\n", t, immediate);
    printf(">>> $%d = %d\n", t, (immediate << 16));
  }
}

void ori(int t, int s,int16_t immediate, int trace_mode){
  if (t != 0) registers[t] = (registers[s] | immediate);
  if (trace_mode){
    printf("ori  $%d, $%d, %d\n", t, s, immediate);
    printf(">>> $%d = %d\n", t, registers[t]);
  }
}
