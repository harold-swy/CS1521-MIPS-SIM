#ifndef PROCESSOR_H
#define PROCESSOR_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>



void syscall(int trace_mode);
void add(int d, int s, int t, int trace_mode);
void sub(int d, int s, int t, int trace_mode);
void slt(int d, int s, int t, int trace_mode);
void mul(int d, int s, int t, int trace_mode);
int beq(int t, int s,int16_t immediate, int trace_mode, int pc);
int bne(int t, int s,int16_t immediate, int trace_mode, int pc);
void addi(int t, int s,int16_t immediate, int trace_mode);
void ori(int t, int s,int16_t immediate, int trace_mode);
void lui(int t, int16_t immediate, int trace_mode);


#endif
