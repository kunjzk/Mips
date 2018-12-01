#ifndef ITYPE_HPP_
#define ITYPE_HPP_

#include "sim.hpp"

int i_type(sim& simulator, uint32_t instr);
int lw(sim& simulator, int rs, int rd, int32_t imm);
int addi(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int addiu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int andi(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int beq(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int bne(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int lbu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int lb(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int lhu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int lh(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int lui(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int lwl(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int lwr(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int ori(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int xori(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int slti(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int sltiu(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int sb(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int sh(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int sw(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int bgez(sim& simulator, uint32_t rs, int32_t imm);
int bgezal(sim& simulator, uint32_t rs, int32_t imm);
int bgtz(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int blez(sim& simulator, uint32_t rs, uint32_t rd, int32_t imm);
int bltz(sim& simulator, uint32_t rs, int32_t imm);
int bltzal(sim& simulator, uint32_t rs, int32_t imm);


#endif
