#ifndef JTYPE_HPP_
#define JTYPE_HPP_

#include "sim.hpp"

//note that these are *real* addresses but jumps only occur WITHIN INTSTR_MEM
//so we only use real_instr_mapping
int j_type(sim& simulator, uint32_t instr);
int j(sim& simulator, uint32_t address);
int jal(sim& simulator, uint32_t address);

#endif
