#ifndef SIM_HPP_
#define SIM_HPP_

#include <vector>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdint.h>
#include <string>
#include <bitset>

//error stuff
#define ARITHMETIC -10 //246
#define MEMORY -11 //245
#define INSTRUCTION -12 //244
#define IO -21 //235
#define COMPLETED -30 //226

//memory stuff
#define ADDR_NULL 0
#define ADDR_INSTR 0x10000000
#define ADDR_INSTR_END 0x10FFFFFF
#define ADDR_DATA 0x20000000
#define ADDR_DATA_END 0x23FFFFFF
#define ADDR_GETC 0x30000000
#define ADDR_PUTC 0x30000004


#define ACTUAL_GETC 0x1400000
#define ACTUAL_PUTC 0x1400001
#define TOTAL_LENGTH 0x1400001

int checkPC(uint32_t PC);
uint32_t create_instr_index(uint32_t PC);
uint32_t create_data_index(uint32_t address);
uint32_t create_index(uint32_t address);


//struct declaration
struct sim {
  int32_t reg[32];
  std::vector<int32_t> mem;
  uint32_t PC;
  uint32_t nextPC;
  int32_t LO;
  int32_t HI;
};

#endif
