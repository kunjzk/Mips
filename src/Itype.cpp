#include "Itype.hpp"

int i_type(sim& simulator, uint32_t instr){
  //std::cout << "entered i type" << std::endl;
  uint32_t opcode = instr >> 26;
  int rs = (instr & 0x03E00000) >> 21;
	int rd = (instr & 0x001F0000) >> 16;
	int32_t im = instr & 0x0000FFFF; //unextended
  int32_t imm; //extended

  //sign extend imm
  if(im>>15 == 1){
    imm = 0xFFFF0000 | im;
  }
  else{
    imm = im;
  }

  int ret_value;

  switch(opcode){
    case 0x00000023:
    	ret_value = lw(simulator, rs, rd, imm);
    break;

    case 0x00000008:
		ret_value = addi(simulator, rs, rd, imm);
	break;

	case 0x00000009:
		ret_value = addiu(simulator, rs, rd, imm);
	break;

	case 0x0000000C:
    imm = im;
		ret_value = andi(simulator, rs, rd, imm);
	break;

	case 0x00000004:
		ret_value = beq(simulator, rs, rd, imm);
	break;

	case 0x00000005:
		ret_value =	bne(simulator, rs, rd, imm);
	break;

	case 0x00000024:
		ret_value =	lbu(simulator, rs, rd, imm);
	break;

	case 0x00000020:
		ret_value =	lb(simulator, rs, rd, imm);
	break;

	case 0x00000025:
		ret_value =	lhu(simulator, rs, rd, imm);
	break;

	case 0x00000021:
		ret_value =	lh(simulator, rs, rd, imm);
	break;

	case 0x0000000F:
		ret_value =	lui(simulator, rs, rd, imm);
	break;

	case 0x00000022:
		ret_value =	lwl(simulator, rs, rd, imm);
	break;

	case 0x00000026:
		ret_value =	lwr(simulator, rs, rd, imm);
	break;

	case 0x00000028:
		ret_value =	sb(simulator, rs, rd, imm);
	break;

	case 0x00000029:
		ret_value =	sh(simulator, rs, rd, imm);
	break;

	case 0x0000002B:
		ret_value =	sw(simulator, rs, rd, imm);
	break;

	case 0x0000000A:
		ret_value =	slti(simulator, rs, rd, imm);
	break;

	case 0x0000000B:
		ret_value =	sltiu(simulator, rs, rd, imm);
	break;

	case 0x0000000E:
    imm = im;
		ret_value =	xori(simulator, rs, rd, imm);
	break;

	case 0x0000000D:
    imm = im;
		ret_value =	ori(simulator, rs, rd, imm);
	break;

	case 0x00000007:
		ret_value =	bgtz(simulator, rs, rd, imm);
	break;

	case 0x00000006:
		ret_value =	blez(simulator, rs, rd, imm);
	break;

	case 0x000001:
		if(rd == 0){
			ret_value = bltz(simulator, rs, imm);
		}
		else if(rd == 1){
			ret_value = bgez(simulator, rs, imm);
		}
		else if(rd == 17){
			ret_value = bgezal(simulator, rs, imm);
		}
		else if( rd == 16){
			ret_value = bltzal(simulator, rs, imm);
		}
		else{
			ret_value = INSTRUCTION;
		}


    default: ret_value = 0;
    break;
  }

  return ret_value;
}

int lw(sim& simulator, int rs, int rd, int32_t imm){
		uint32_t address = static_cast<uint32_t>(simulator.reg[rs] + imm);

    //checking validity..
    if(address %4 != 0 || ((address<ADDR_INSTR || address > ADDR_INSTR_END) && (address < ADDR_DATA || address > ADDR_DATA_END) && address != ADDR_GETC)){
      return MEMORY;
    }

    //if we need to read from input..
    if(address == ADDR_GETC){
      char input = std::getchar();
      if(!std::cin.good()){
        return IO;
      }
      else if(input == EOF){
        simulator.reg[rd] = 0xFFFFFFFF;
      }
      else{
        simulator.reg[rd] = (0x00000000 | input);
      }
    }

    //if we're reading from memory..
    else{
				simulator.reg[rd] = simulator.mem[create_index(address)];
			}

    simulator.nextPC +=4;
    return 0;
}



int addi(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
		int32_t temp = simulator.reg[rs];
		if (((temp < 0) && (imm < 0) && (temp + imm >= 0)) || ((temp > 0) && (imm > 0) && (temp + imm <= 0))){
			return ARITHMETIC;
		}
		else {
			simulator.reg[rd] = temp + imm;
		}

		simulator.nextPC +=4;
		return 0;
}

int addiu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	simulator.reg[rd] = static_cast<uint32_t>(static_cast<uint32_t>(simulator.reg[rs]) + imm);
	simulator.nextPC +=4;
	return 0;
}

int andi(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	int32_t temp = simulator.reg[rs];
	simulator.reg[rd] = temp & imm;
	simulator.nextPC +=4;
	return 0;
}

int beq(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	if(simulator.reg[rd] == simulator.reg[rs]){
		simulator.nextPC += 4*imm;
	}
	else{
		simulator.nextPC +=4;

	}
	return 0;
}

int bne(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	if(simulator.reg[rd] != simulator.reg[rs]){
		simulator.nextPC += 4*imm;
	}
	else{
		simulator.nextPC +=4;

	}
	return 0;
}

int lbu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
  uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
  if(((address<ADDR_INSTR || address > ADDR_INSTR_END) && (address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_GETC && address <= ADDR_GETC + 3)))
  {
    return MEMORY;
  }

		if(address >= ADDR_GETC && address <= ADDR_GETC + 3){
      char c;
      c = std::getchar();
      if(!std::cin.good()){
        return IO;
      }
      else if(c==EOF){
        simulator.reg[rd] = 0x000000FF; //If we have EOF set rd to -1
        simulator.nextPC +=4;
        return 0;
      }
      else {
        if((address % 4) != 3){
  				simulator.reg[rd] = 0;
  			}
        else{
          simulator.reg[rd] = c & 0x000000FF;
        }
      }
		}

    else{
      switch(address % 4){
  			case 0:
  				simulator.reg[rd] = ((simulator.mem[create_index(address)] & 0xFF000000) >> 24) & 0x000000FF;
  			break;

  			case 1:
  				simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x00FF0000) >> 16;
  			break;

  			case 2:
  				simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x0000FF00) >> 8;
  			break;

  			case 3:
  				simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x000000FF);
  			break;
  		}
    }

		simulator.nextPC +=4;
		return 0;
}

int lb(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
  uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
  if(((address<ADDR_INSTR || address > ADDR_INSTR_END) && (address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_GETC && address <= ADDR_GETC + 3)))
  {
    return MEMORY;
  }

		if(address >= ADDR_GETC && address <= ADDR_GETC + 3){
      char c;
      c = std::getchar();
      if(!std::cin.good()){
        return IO;
      }
      else if(c==EOF){
        simulator.reg[rd] = 0xFFFFFFFF; //If we have EOF set rd to -1
        simulator.nextPC +=4;
        return 0;
      }
      else {
        if((address % 4) != 3){
  				simulator.reg[rd] = 0;
  			}
        else{
          simulator.reg[rd] = c & 0x000000FF;
        }
      }
		}

    else{
      switch(address % 4){
  			case 0:
  				simulator.reg[rd] = ((simulator.mem[create_index(address)] & 0xFF000000) >> 24) & 0x000000FF;
  			break;

  			case 1:
  				simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x00FF0000) >> 16;
  			break;

  			case 2:
  				simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x0000FF00) >> 8;
  			break;

  			case 3:
  				simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x000000FF);
  			break;
  		}
    }

		if(simulator.reg[rd] >> 7 == 1){
			simulator.reg[rd] = 0xFFFFFF00 | simulator.reg[rd];
		}

		simulator.nextPC +=4;
		return 0;
	}

int lhu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
  uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
  if(((address%2 != 0) || ((address<ADDR_INSTR || address > ADDR_INSTR_END) && (address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_GETC && address <= ADDR_GETC + 3))))
  {
    return MEMORY;
  }

  if(address >= ADDR_GETC && address <= ADDR_GETC + 3){
    char c;
    c = std::getchar();
    if(!std::cin.good()){
      return IO;
    }
    else if(c==EOF){
      simulator.reg[rd] = 0x0000FFFF; //If we have EOF set rd to -1
      simulator.nextPC +=4;
      return 0;
    }
    else{
      if(address%4 == 0){
        simulator.reg[rd] = 0;
      }
      if(address%4 == 2){
        simulator.reg[rd] = c & 0x000000FF;
      }
    }
  }

  else{
    switch(address % 4){
      case 0:
        simulator.reg[rd] = ((simulator.mem[create_index(address)] & 0xFFFF0000) >> 16) & 0x0000FFFF;
      break;

      case 2:
        simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x0000FFFF);
      break;
    }
  }

  simulator.nextPC +=4;
  return 0;
}


int lh(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
  uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
  if(((address%2 != 0) || ((address<ADDR_INSTR || address > ADDR_INSTR_END) && (address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_GETC && address <= ADDR_GETC + 3))))
  {
    return MEMORY;
  }

  if(address >= ADDR_GETC && address <= ADDR_GETC + 3){
    char c;
    c = std::getchar();
    if(!std::cin.good()){
      return IO;
    }
    else if(c==EOF){
      simulator.reg[rd] = 0xFFFFFFFF; //If we have EOF set rd to -1
      simulator.nextPC +=4;
      return 0;
    }
    else{
      if(address%4 == 0){
        simulator.reg[rd] = 0;
      }
      if(address%4 == 2){
        simulator.reg[rd] = c & 0x000000FF;
      }
    }
  }

  else{
    switch(address % 4){
      case 0:
        simulator.reg[rd] = ((simulator.mem[create_index(address)] & 0xFFFF0000) >> 16) & 0x0000FFFF;
      break;

      case 2:
        simulator.reg[rd] = (simulator.mem[create_index(address)] & 0x0000FFFF);
      break;
    }
  }

  if((simulator.reg[rd] >> 15) == 1){
			simulator.reg[rd] = 0xFFFF0000 | simulator.reg[rd];
	}

	simulator.nextPC +=4;
	return 0;

}



int lui(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
		if(rs != 0x00000000){
			return INSTRUCTION;
		}
		else{
			imm = imm << 16;
			simulator.reg[rd] = imm;
		}
		simulator.nextPC +=4;
		return 0;
	}



int lwl(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
  uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
  uint32_t intermediate = 0;
  if(((address<ADDR_INSTR || address > ADDR_INSTR_END) && (address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_GETC && address <= ADDR_GETC + 3)))
  {
    return MEMORY;
  }

  //for a cheeky LWL at GETC
  //Set GETC up like a normal data word then do LWL
  if(address >= ADDR_GETC && address <= ADDR_GETC+3){
    char c = std::getchar();
    if(!std::cin.good()){
      return IO;
    }
    if(c==EOF){
      simulator.mem[ACTUAL_GETC] = 0xFFFFFFFF;
    }
    else{
      simulator.mem[ACTUAL_GETC] = 0x00000000 | (c & 0x000000FF);
    }

    switch(address % 4){
  		case 0:
  			simulator.reg[rd] = simulator.mem[ACTUAL_GETC];
  		break;

  		case 1:
  			intermediate = (simulator.mem[ACTUAL_GETC] & 0x00FFFFFF) << 8;
  			simulator.reg[rd] = ((simulator.reg[rd] & 0x000000FF)| intermediate);
  		break;

  		case 2:
  			intermediate = (simulator.mem[ACTUAL_GETC] & 0x0000FFFF) << 16;
  			simulator.reg[rd] = ((simulator.reg[rd] & 0x0000FFFF)| intermediate);
  		break;

  		case 3:
  			intermediate = (simulator.mem[ACTUAL_GETC] & 0x000000FF) << 24;
  			simulator.reg[rd] = ((simulator.reg[rd] & 0x00FFFFFF)| intermediate);
  		break;
  	}

  }

  else{
    switch(address % 4){
  		case 0:
  			simulator.reg[rd] = simulator.mem[create_index(address)];
  		break;

  		case 1:
  			intermediate = (simulator.mem[create_index(address)] & 0x00FFFFFF) << 8;
  			simulator.reg[rd] = ((simulator.reg[rd] & 0x000000FF)| intermediate);
  		break;

  		case 2:
  			intermediate = (simulator.mem[create_index(address)] & 0x0000FFFF) << 16;
  			simulator.reg[rd] = ((simulator.reg[rd] & 0x0000FFFF)| intermediate);
  		break;

  		case 3:
  			intermediate = (simulator.mem[create_index(address)] & 0x000000FF) << 24;
  			simulator.reg[rd] = ((simulator.reg[rd] & 0x00FFFFFF)| intermediate);
  		break;
  	}
  }

  //std::cout << std::hex << simulator.reg[rd] << std::dec << std::endl;
	simulator.nextPC +=4;
	return 0;
}

int lwr(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
  uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
  uint32_t intermediate = 0;
  if(((address<ADDR_INSTR || address > ADDR_INSTR_END) && (address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_GETC && address <= ADDR_GETC + 3)))
  {
    return MEMORY;
  }

  if(address >= ADDR_GETC && address <= ADDR_GETC+3){
    char c = std::getchar();
    if(!std::cin.good()){
      return IO;
    }
    if(c==EOF){
      simulator.mem[ACTUAL_GETC] = 0xFFFFFFFF;
    }
    else{
      simulator.mem[ACTUAL_GETC] = 0x00000000 | (c & 0x000000FF);
    }

    switch(address % 4){
  		case 0:
  			intermediate = (((simulator.mem[ACTUAL_GETC] & 0xFF000000) >> 24) & 0x000000FF);
  			simulator.reg[rd] = (simulator.reg[rd] & 0xFFFFFF00) | intermediate;
  		break;

  		case 1:
  			intermediate = (((simulator.mem[ACTUAL_GETC] & 0xFFFF0000) >> 16) & 0x0000FFFF);
  			simulator.reg[rd] = (simulator.reg[rd] & 0xFFFF0000) | intermediate;
  		break;

  		case 2:
  			intermediate = (((simulator.mem[ACTUAL_GETC] & 0xFFFFFF00) >> 8) & 0x00FFFFFF);
  			simulator.reg[rd] = (simulator.reg[rd] & 0xFF000000) | intermediate;
  		break;

  		case 3:
  			simulator.reg[rd] = simulator.mem[ACTUAL_GETC];
  		break;
  }
}

    else{
	switch(address % 4){
		case 0:
			intermediate = (((simulator.mem[create_index(address)] & 0xFF000000) >> 24) & 0x000000FF);
			simulator.reg[rd] = (simulator.reg[rd] & 0xFFFFFF00) | intermediate;
		break;

		case 1:
			intermediate = (((simulator.mem[create_index(address)] & 0xFFFF0000) >> 16) & 0x0000FFFF);
			simulator.reg[rd] = (simulator.reg[rd] & 0xFFFF0000) | intermediate;
		break;

		case 2:
			intermediate = (((simulator.mem[create_index(address)] & 0xFFFFFF00) >> 8) & 0X00FFFFFF);
			simulator.reg[rd] = (simulator.reg[rd] & 0xFF000000) | intermediate;
		break;

		case 3:
			simulator.reg[rd] = simulator.mem[create_index(address)];
		break;

	}
     }

  //std::cout << std::hex << simulator.reg[rd] << std::dec << std::endl;
	simulator.nextPC +=4;
	return 0;

}

int ori(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	simulator.reg[rd] = simulator.reg[rs] | imm;
  //std::cout << std::bitset<32>(simulator.reg[rd]) << std::endl;
	simulator.nextPC +=4;
	return 0;
}

int xori(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	simulator.reg[rd] = simulator.reg[rs] ^ imm;

	simulator.nextPC +=4;
	return 0;
}

int slti(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	if(simulator.reg[rs] < imm){
		simulator.reg[rd] = 1;
	}
	else{
		simulator.reg[rd] = 0;
	}

	simulator.nextPC +=4;
	return 0;
}

int sltiu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
	uint32_t temp = static_cast<uint32_t>(imm);

	if(uint32_t(simulator.reg[rs]) < temp){
		simulator.reg[rd] = 1;
	}
	else{
		simulator.reg[rd] = 0;
	}

	simulator.nextPC +=4;
	return 0;
}

int sb(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
		uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
		if((address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_PUTC && address <= ADDR_PUTC + 3))
		{
			return MEMORY;
		}

    //if you want I/O
		if(address >= ADDR_PUTC && address <= ADDR_PUTC + 3)
		{
			if((address % 4) != 3){
			     std::putchar(static_cast<char>(0));
           simulator.nextPC +=4;
           return 0;
			}
			else{
        char c = 0x000000FF & simulator.reg[rd];
				std::putchar(c);
				if(!std::cout.good()){
					return IO;
				}
        simulator.nextPC +=4;
				return 0;
			}
		}

		int32_t value = simulator.reg[rd] & 0x000000FF;
		uint32_t index = address % 4;
		switch(index){
			case 0:
				simulator.mem[create_data_index(address)] = (simulator.mem[create_data_index(address)] & 0x00FFFFFF) | (value << 24);
			break;

			case 1:
				simulator.mem[create_data_index(address)] = (simulator.mem[create_data_index(address)] & 0xFF00FFFF) | (value << 16);
			break;

			case 2:
				simulator.mem[create_data_index(address)] = (simulator.mem[create_data_index(address)] & 0xFFFF00FF) | (value << 8);
			break;

			case 3:
				simulator.mem[create_data_index(address)] = (simulator.mem[create_data_index(address)] & 0xFFFFFF00) | value;
			break;
		}
		simulator.nextPC +=4;
		return 0;
	}

int sh(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
		uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
		if((address < ADDR_DATA || address > ADDR_DATA_END) && !(address >= ADDR_PUTC && address <= ADDR_PUTC + 3))
		{
			return MEMORY;
		}

    //if you want I/O
		if(address >= ADDR_PUTC && address <= ADDR_PUTC + 3){
			if(address % 4 == 1 || address % 4 == 3){
				return MEMORY;
			}

			else if(address%4 == 2){
        char c = static_cast<char>(0x000000FF & simulator.reg[rd]);
				std::putchar(c);
				if(!std::cout.good()){
					return IO;
				}
        simulator.nextPC +=4;
				return 0;
			}

      else{
        std::putchar(static_cast<char>(0));
        if(!std::cout.good()){
					return IO;
				}
        simulator.nextPC +=4;
        return 0;
      }
		}

    //just writing to normal data memory
		if(address % 2) return MEMORY;

		else{
			switch(address % 4){
				case 0:
					simulator.mem[create_data_index(address)] = (simulator.mem[create_data_index(address)] & 0x0000FFFF) |((simulator.reg[rd] & 0x0000FFFF) << 16);
				break;
				case 2:
					simulator.mem[create_data_index(address)] = (simulator.mem[create_data_index(address)] & 0xFFFF0000) | (simulator.reg[rd] & 0x0000FFFF);
				break;
			}
		}

		simulator.nextPC +=4;
		return 0;
	}

int sw(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
		uint32_t address = static_cast<uint32_t>(imm + simulator.reg[rs]);
		if((address < ADDR_DATA || address > ADDR_DATA_END) && (address != ADDR_PUTC))
		{
			return MEMORY;
		}

		if(address % 4) return MEMORY;

		else{
			if(address == ADDR_PUTC){
        char c = static_cast<char>(0x000000FF & simulator.reg[rd]);
				std::putchar(c);
				if(!std::cout.good()){
					return IO;
				}
			}
      else{
        simulator.mem[create_data_index(address)] = simulator.reg[rd];
      }
		}

		simulator.nextPC +=4;
		return 0;
	}

int bgez(sim& simulator, uint32_t rs, int32_t imm){
	if(simulator.reg[rs] >= 0){
		simulator.nextPC += 4*imm;
	}
	else{
		simulator.nextPC +=4;
	}
	return 0;
}

int bgezal(sim& simulator, uint32_t rs, int32_t imm){
	int32_t temp = simulator.reg[rs];
	//Regardless of whether the condition is true, the return address needs to be set
	simulator.reg[31] = (simulator.PC) + 8;
	if(temp >= 0){
		simulator.nextPC += 4*imm;
	}
	else{
		simulator.nextPC +=4;
	}
	return 0;
}

int bgtz(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
		if(rd != 0x00000000){
			return INSTRUCTION;
		}
		else{
			if(simulator.reg[rs] > 0){
				simulator.nextPC += 4*imm;
			}
			else{
				simulator.nextPC +=4;
			}
			return 0;
		}
	}

int blez(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm){
		int32_t temp = simulator.reg[rs];
		if(rd != 0){
			return INSTRUCTION;
		}

		else{
			if(temp <= 0x00000000){
				simulator.nextPC += 4*imm;
			}
			else{
				simulator.nextPC +=4;
			}
			return 0;
		}
	}

int bltz(sim& simulator, uint32_t rs, int32_t imm){
	int32_t temp = simulator.reg[rs];
	if(temp < 0){
		simulator.nextPC += 4*imm;
	}
	else{
		simulator.nextPC +=4;
	}
	return 0;
}

int bltzal(sim& simulator, uint32_t rs, int32_t imm){
	int32_t temp = simulator.reg[rs];
	//Regardless of whether the conditions is true, the return address has to be set
	simulator.reg[31] = (simulator.PC) + 8;
	if(temp < 0){
		simulator.nextPC += 4*imm;
	}
	else{
		simulator.nextPC +=4;
	}
	return 0;
}
