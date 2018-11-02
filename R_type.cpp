#define ARITHMETIC -10;


int r_type(struct& simulator, uint32_t instr){
  int rs = (instr >> 21) & 0x0000001F;
  int rs2 = (instr >> 16) & 0x0000001F;
  int rd = (instr >> 11) & 0x0000001F;
  int fn = instr & 0x0000003F;

  //at this point do we check that rs, rd, rs2 are non-zero?

  switch(fn):
    //case default: throw exception?
    //performs opertation depending on function code
    case 0x00000020: add(simulator, rs, rs2, rd);
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
    break;`

}

int add (struct& simulator, int rs, int rs2, int rd){
  int32_t a = simulator.reg[rs];
  int32_t b = simulator.reg[rs2];
  int32_t result = a + b;

  if (a>0 && b>0 && result <= 0 || b<0 && a<0 && result => 0){
    return ARITHMETIC;
  }

  simulator.reg[rd] = result;


}
