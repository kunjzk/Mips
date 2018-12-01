#include "Rtype.hpp"

int r_type(sim& simulator, uint32_t instr){
  //std::cout << "entered r type" << std::endl;
  int rs = (instr >> 21) & 0x0000001F;
  int rs2 = (instr >> 16) & 0x0000001F;
  int rd = (instr >> 11) & 0x0000001F;
  int fn = instr & 0x0000003F;
  int shamt = (instr >> 6);

  int ret_value;

  //at this point do we check that rs, rd, rs2 are non-zero?

  switch(fn){
    case 0x00000020: ret_value = add(simulator, rs, rs2, rd); //done
    break;
    case 0x00000021: ret_value = addu(simulator, rs, rs2, rd); //done
    break;
    case 0x00000024: ret_value = AND(simulator, rs, rs2, rd); //done
    break;
    case 0x0000001A: ret_value = div(simulator, rs, rs2); //done
    break;
    case 0x0000001B: ret_value = divu(simulator, rs, rs2); //done
    break;
    case 0x00000009: ret_value = jalr(simulator, rs, rd); //done
    break;
    case 0x00000008: ret_value = jr(simulator, rs); //done
    break;
    case 0x00000010: ret_value = mfhi(simulator, rd); //done
    break;
    case 0x00000012: ret_value = mflo(simulator, rd); //done
    break;
    case 0x00000011: ret_value = mthi(simulator, rs);
    break;
    case 0x00000013: ret_value = mtlo(simulator, rs);
    break;
    case 0x00000018: ret_value = mult(simulator, rs, rs2); //done
    break;
    case 0x00000019: ret_value = multu(simulator, rs, rs2); //done
    break;
    case 0x00000025: ret_value = OR(simulator, rs, rs2, rd); //done
    break;
    case 0x0000002A: ret_value = slt(simulator, rs, rs2, rd); //done
    break;
    case 0x0000002B: ret_value = sltu(simulator, rs, rs2, rd); //done
    break;
    case 0x00000003: ret_value = sra(simulator, rs2, shamt, rd); //done
    break;
    case 0x00000007: ret_value = srav(simulator, rs, rs2, rd); //done
    break;
    case 0x00000002: ret_value = srl(simulator, rs2, shamt, rd); //done
    break;
    case 0x00000006: ret_value = srlv(simulator, rs, rs2, rd); //done
    break;
    case 0x00000000: ret_value = sll(simulator, rs2, shamt, rd);
    break;
    case 0x00000004: ret_value = sllv(simulator, rs, rs2, rd);
    break;
    case 0x00000022: ret_value = sub(simulator, rs, rs2, rd); //done
    break;
    case 0x00000023: ret_value = subu(simulator, rs, rs2, rd); //done
    break;
    case 0x00000026: ret_value = XOR(simulator, rs, rs2, rd); //done
    break;
    default: ret_value= INSTRUCTION;
    break;
}

  return ret_value;

}

int add (sim& simulator, int rs, int rs2, int rd){
  int32_t a = simulator.reg[rs];
  int32_t b = simulator.reg[rs2];
  int32_t result = a + b;

  if ((a>0 && b>0 && result <= 0) || (b<0 && a<0 && result >= 0)){
    return ARITHMETIC;
  }
  simulator.reg[rd] = result;
  simulator.nextPC+=4;
  return 0;
}

int addu(sim& simulator, int rs, int rs2, int rd){
  uint32_t op1 = simulator.reg[rs];
  uint32_t op2 = simulator.reg[rs2];
  simulator.reg[rd] = static_cast<uint32_t>(op1+op2);
  simulator.nextPC+=4;
  return 0;
}

int AND(sim& simulator, int rs, int rs2, int rd){
  simulator.reg[rd] = (simulator.reg[rs] & simulator.reg[rs2]);
  simulator.nextPC+=4;
  return 0;
}

int divu (sim& simulator, int rs, int rs2){
  uint32_t op1 = static_cast<uint32_t>(simulator.reg[rs]);
  uint32_t op2 = static_cast<uint32_t>(simulator.reg[rs2]);
  if(op2==0){
    //divide by 0 is not an exception
    simulator.nextPC+=4;
    return 0;
  }

  simulator.LO = static_cast<uint32_t>(op1/op2);
  simulator.HI = static_cast<uint32_t>(op1%op2);
  simulator.nextPC+=4;
  return 0;
}

int div(sim& simulator, int rs, int rs2){
  int32_t op1 = simulator.reg[rs];
  int32_t op2 = simulator.reg[rs2];

  if(op2==0){
    simulator.nextPC+=4;
    return 0;
  }

  simulator.LO = op1/op2;
  simulator.HI = op1%op2;
  simulator.nextPC+=4;
  return 0;
}

int jalr(sim& simulator, int rs, int rd){
    if(rd == 0){
        rd = 31;
    }

  simulator.nextPC = simulator.reg[rs];
  simulator.reg[rd] = simulator.PC + 8;

  if(simulator.reg[rs]%4 != 0){
    simulator.nextPC = ADDR_DATA;
    //set nextPC = address data, which is invalid but ensures that we only
    //throw an exception after the branch delay
  }
	return 0;
}

int jr(sim& simulator, int rs){
  if(simulator.reg[rs]%4 != 0){
    return MEMORY;
  }
  else{
    simulator.nextPC = simulator.reg[rs];
  }
	return 0;
}

int mfhi(sim& simulator, int rd){
  simulator.reg[rd] = simulator.HI;
  simulator.nextPC+=4;
  return 0;
}

int mflo(sim& simulator, int rd){
  simulator.reg[rd] = simulator.LO;
  simulator.nextPC+=4;
  return 0;
}

int mthi(sim& simulator, int rs){
  simulator.HI = simulator.reg[rs];
  simulator.nextPC+=4;
  return 0;
}

int mtlo(sim& simulator, int rs){
  //std::cout << rs << std::endl;
  //std::cout << std::bitset<32>(simulator.reg[rs]) << std::endl;
  simulator.LO = simulator.reg[rs];
  //std::cout << std::bitset<32>(simulator.LO) << std::endl;
  simulator.nextPC+=4;
  return 0;
}

int mult(sim& simulator, int rs, int rs2)
{
  int64_t factor1 = simulator.reg[rs];
  int64_t factor2 = simulator.reg[rs2];
  int64_t product  = factor1 * factor2;
  simulator.HI = (product >> 32);
  simulator.LO = (product << 32) >> 32;
  simulator.nextPC+=4;
  return 0;
}

int multu(sim& simulator, int rs, int rs2)
{
  uint64_t factor1 = (uint32_t)simulator.reg[rs];
  uint64_t factor2 = (uint32_t)simulator.reg[rs2];
  uint64_t product = factor1 * factor2;
  simulator.HI = (product >> 32);
  simulator.LO = (product << 32) >> 32;
  simulator.nextPC+=4;
 return 0;
}
int OR(sim& simulator, int rs, int rs2, int rd)
{
  simulator.reg[rd] = (simulator.reg[rs] | simulator.reg[rs2]);
  simulator.nextPC+=4;
  return 0;
}
int slt(sim& simulator, int rs, int rs2, int rd){
  int32_t temp1 = simulator.reg[rs];
  int32_t temp2 = simulator.reg[rs2];
  if(temp1 < temp2){
    simulator.reg[rd] = 1;
  }
  else{
    simulator.reg[rd] = 0;
  }
  simulator.nextPC+=4;
  return 0;
}

int sltu(sim& simulator, int rs, int rs2, int rd){
uint32_t r1 = static_cast<uint32_t>(simulator.reg[rs]);
uint32_t r2 = static_cast<uint32_t>(simulator.reg[rs2]);

  if((0|r1) < (0|r2)) {
    simulator.reg[rd] = ((0|r1) < (0|r2)) | 1;
  }
  else {
    simulator.reg[rd] = 0;
  }
  simulator.nextPC+=4;
  return 0;
}
int sra(sim& simulator, int rs2, int shamt, int rd)
{
  int32_t operand = simulator.reg[rs2];
  simulator.reg[rd] = (operand >> shamt);
  simulator.nextPC+=4;
  return 0;
}

int srav(sim& simulator, int rs, int rs2, int rd)
{
  simulator.reg[rd] = ((int32_t)simulator.reg[rs2] >> simulator.reg[rs]);
  simulator.nextPC+=4;
  return 0;
}
int srl(sim& simulator, int rs2, int shamt, int rd)
{
  uint32_t operand = static_cast<uint32_t>(simulator.reg[rs2]);
  simulator.reg[rd] = static_cast<uint32_t>(operand >> shamt);
  simulator.nextPC+=4;
  return 0;
}

int srlv(sim& simulator, int rs, int rs2, int rd)
{
  uint32_t operand = simulator.reg[rs2];
  simulator.reg[rd]  = (static_cast<uint32_t>(operand) >> simulator.reg[rs]);
  simulator.nextPC+=4;
  return 0;
}

int sll(sim& simulator, int rs2, int shamt, int rd)
{
  simulator.reg[rd] = (simulator.reg[rs2] << shamt);
  simulator.nextPC+=4;
  return 0;
}

int sllv(sim& simulator, int rs, int rs2, int rd)
{
  simulator.reg[rd] = (simulator.reg[rs2] << simulator.reg[rs]);
  simulator.nextPC+=4;
  return 0;
}

int sub(sim& simulator, int rs, int rs2, int rd)
{
  int32_t var1 = simulator.reg[rs];
  int32_t subtractor = simulator.reg[rs2];
  int32_t result = var1 - subtractor;
    if (((var1 <= 0 ) && (subtractor > 0) && (result >= 0)) || ((var1 >= 0) && (subtractor < 0) && (result <= 0))){
      return ARITHMETIC;
    }
    else {
      simulator.reg[rd] =  var1 - subtractor;
    }
    simulator.nextPC+=4;
    return 0;
}
int subu(sim& simulator, int rs, int rs2, int rd)
{
  simulator.reg[rd] =  static_cast<uint32_t>(static_cast<uint32_t>(simulator.reg[rs]) - static_cast<uint32_t>(simulator.reg[rs2]));
  simulator.nextPC+=4;
  return 0;
}

int XOR(sim& simulator, int rs, int rs2, int rd)
{
  simulator.reg[rd] = (simulator.reg[rs] ^ simulator.reg[rs2]);
  simulator.nextPC+=4;
  return 0;
}
