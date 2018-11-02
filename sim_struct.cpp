#include <iostream>
#include <vector>

#define INTSTR_MEM_SIZE 0x400000
#define DATA_MEM_SIZE 0x1000000
#define MEM_SIZE 0x1400000


struct sim {
  int32_t reg[32];
  std::vector<int32_t> mem;
  uint32_t pc;
};

int main(){
  sim sim1;
  sim1.mem.resize(MEM_SIZE, 0);
  std::cout << sizeof(sim1.mem) << std::endl;
  std::cout << std::bitset<32> (sim1.mem[0]) <<std::endl;
}
