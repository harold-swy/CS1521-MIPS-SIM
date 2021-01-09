#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "registers.h"


//this file initiates a array of 32 registers then sets all registers to 0
//initiateRegisters() to be called before processing the first instruction i.e. at the beginning of execute_instructions

uint32_t registers[REGISTER_COUNT];

void initiateRegisters(){
  for (int x = 0; x <REGISTER_COUNT; x++)
  {
    registers[x] = 0;
  }
  return;
}
