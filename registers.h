#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#define REGISTER_COUNT 32

extern uint32_t registers[REGISTER_COUNT];
void initiateRegisters();

#endif
