#include "Jtype.cpp"
#include "Itype.cpp"
#include "Rtype.cpp"
#include "sim.cpp"


int main(int argc, char* argv[]){
  //std::cout<<"hello"<<std::endl;

  sim simulator;
  simulator.PC = ADDR_INSTR;
  simulator.nextPC = simulator.PC + 4;
  //std::cout << "setting vector" << std::endl;
  simulator.mem.resize(TOTAL_LENGTH);
  //std::cout << "vector set" << std::endl;
  simulator.LO = 0;
  simulator.HI = 0;
  for(int i=0; i<32; i++){
      simulator.reg[i] = 0;
  }

//------ LOAD INSTRUCTIONS INTO MEMORY ----------
  if(argc !=2){
    //std::cerr << "No second argument" << std::endl;
    std::exit(IO);
  }

  const char* filename = argv[1];
  std::ifstream bin(filename, std::ios::out | std::ios::binary | std::ios::ate);

  if(!bin.is_open()){
      //std::cout << "Could not open file." << std::endl;
      std::exit(IO);
  }

  int length = bin.tellg();
  bin.seekg (0, std::ios::beg);
  char* buffer = new char [length];
  bin.read(buffer, length);
  bin.close();
  //std::cout << "instructions fetched from memory: " << std::endl;
  uint32_t inst_address = 0;
  for(int i = 0; i < length; i+=4)
  {
    int32_t instruction = 0;
    for(int j=0; j<4; j++){
      instruction = instruction | (buffer[i+j]&0x000000FF);
      if(j!=3){
        instruction<<=8;
      }
    }
    //std::cout << std::bitset<32>(instruction) << std::endl;
    simulator.mem[inst_address] = instruction;
    inst_address++;
  }

  //std::cout << "-----setup done----" << std::endl;
  while(true){
    int memCheck = checkPC(simulator.PC);

    if(memCheck == MEMORY){
      //std::cout<<"Exited: PC range"<<std::endl;
      std::exit(MEMORY);
    }
    else if(memCheck == COMPLETED){
      int exit_code = simulator.reg[2] & 0x000000FF;
      //std::cout<<"Exited: COMPLETED"<<std::endl;
      std::exit(exit_code);
    }

    //PC contains a valid address. proceed to decode instructions.
    else{

      uint32_t instruction = simulator.mem[create_instr_index(simulator.PC)];
      //std::cout << std::bitset<32>(instruction) << std::endl;
      uint32_t opcode = instruction >> 26;
      uint32_t temp = simulator.nextPC;
      simulator.reg[0] = 0;

      int error_status;

      switch(opcode){
        case 0x00000000:       error_status = r_type(simulator, instruction);
        break;
        case 0x00000002:       error_status = j_type(simulator, instruction);
        break;
        case 0x00000003:       error_status = j_type(simulator, instruction);
        break;
        default:      error_status = i_type(simulator, instruction);
        break;

      }

      if(error_status != 0){
        //std::cout << "Exited: Error during instruction"<<std::endl;
        std::exit(error_status);
      }

      simulator.PC = temp;
    }

  }

  return 0;

}
