#include "sim.hpp"

uint32_t create_instr_index(uint32_t PC){
  uint32_t index = (PC - 0x10000000)/4;
  return index;
}

int checkPC(uint32_t PC){
  if(PC!= ADDR_NULL && (PC > ADDR_INSTR_END || PC < ADDR_INSTR)){
    return MEMORY;
  }
  else if(PC == ADDR_NULL){
    return COMPLETED;
  }
  else{
    //it's within instruction range. all fine.
    return 0;
  }
}

uint32_t create_data_index(uint32_t address){
    uint32_t index = (address - 0x1F000000)/4;
    return index;
}



uint32_t create_index(uint32_t address){
  uint32_t index;
  if(address <= ADDR_INSTR_END && address >= ADDR_INSTR){
     index = (address - 0x10000000)/4;
  }
  else{
     index = (address - 0x1F000000)/4;
  }
  return index;
}
