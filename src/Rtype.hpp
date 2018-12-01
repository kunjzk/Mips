#ifndef RTYPE_HPP_
#define RTYPE_HPP_

#include "sim.hpp"

int r_type(sim& simulator, uint32_t instr);
int add(sim& simulator, int rs, int rs2, int rd);
int addu(sim& simulator, int rs, int rs2, int rd);
int AND(sim& simulator, int rs, int rs2, int rd);
int div(sim& simulator, int rs, int rs2);
int divu(sim& simulator, int rs, int rs2);
int jalr(sim& simulator, int rs, int rd);
int jr(sim& simulator, int rs);
int mfhi(sim& simulator, int rd);
int mflo(sim& simulator, int rd);
int mthi(sim& simulator, int rs);
int mtlo(sim& simulator, int rs);
int mult(sim& simulator, int  rs, int  rs2);
int multu(sim& simulator, int rs, int rs2);
int OR(sim& simulator, int rs, int rs2, int rd);
int slt(sim& simulator, int rs, int rs2, int rd);
int sltu(sim& simulator, int rs, int rs2, int rd);
int sra(sim& simulator, int rs2, int shamt, int rd);
int srav(sim& simulator, int rs, int rs2, int rd);
int srl(sim& simulator, int rs2, int shamt, int rd);
int srlv(sim& simulator, int rs, int rs2, int rd);
int sll(sim& simulator, int rs2, int shamt, int rd);
int sllv(sim& simulator, int rs, int rs2, int rd);
int sub(sim& simulator, int rs, int rs2, int rd);
int subu(sim& simulator, int rs, int rs2, int rd);
int XOR(sim& simulator, int rs, int rs2, int rd);

#endif
