#include <iostream>

int main(){

  uint32_t bin = 0x0AFFAAAC;

  uint32_t opcode = bin >> 26;

  std::cout << std::bitset<32>(bin) <<std::endl;
  std::cout << std::bitset<32>(opcode) << std::endl;

  if(opcode == 0){
    std::cout << "R type" << std::endl;
    uint32_t func_code = bin & 0x0000003F
    r_type(sim, instr)










    switch(func_code):
      case 0x00000020: add;
      break;
      case 0x00000021: addu;
      break;
      case 0x00000024: and;
      break;
      case 0x0000001A: div;
      break;
      case 0x0000001B: divu;
      break;
      case 0x00000009: jalr;
      break;
      case 0x00000008: jr;
      break;
      case 0x00000010: mfhi;
      break;
      case 0x00000012: mflo;
      break;
      case 0x00000011: mthi;
      break;
      case 0x00000013: mtlo;
      break;
      case 0x00000018: mult;
      break;
      case 0x00000019: multu;
      break;
      case 0x00000025: or;
      break;
      case 0x0000002A: slt;
      break;
      case 0x0000002B: sltu;
      break;
      case 0x00000003: sra;
      break;
      case 0x00000007: srav;
      break;
      case 0x00000002: srl;
      break;
      case 0x00000006: srlv;
      break;
      case 0x00000000: sll;
      break;
      case 0x00000008: sllv;
      break;
      case 0x00000022: sub;
      break;
      case 0x00000023: subu;
      break;
      case 0x00000026: xor;
      break;



  }

  else if(opcode == 3 || opcode == 2){
    std::cout << "J type" << std::endl;
  }

  else{
    std::cout << "I type" << std::endl;
  }


}
