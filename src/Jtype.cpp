#include "Jtype.hpp"

int j_type(sim& simulator, uint32_t instr){
  //std::cout << "Entered j type" << std::endl;
  uint32_t opcode = (instr >> 26);
  uint32_t address = (instr & 0x03FFFFFF);

  int ret_value;

  switch(opcode){
    case 0x00000002: ret_value = j(simulator, address);
    break;
    case 0x00000003: ret_value = jal(simulator, address);
    break;
    default: ret_value = INSTRUCTION;
    break;
}


  return ret_value;

}
int j(sim& simulator, uint32_t address){
  simulator.nextPC = ((simulator.PC&0xF0000000) | address<<2);
  return 0;
}

int jal(sim& simulator, uint32_t address){
  simulator.reg[31] = simulator.PC + 8;
  simulator.nextPC = ((simulator.PC&0xF0000000) | address<<2);
  return 0;
}

//CHECK IF THE ADDRESSES ARE VALID
