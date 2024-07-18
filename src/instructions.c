#include "instructions.h"
#include "cpu.h"
#include "gb.h"

#define READ8(addr)  gb_read8(cpu->gb, addr)
#define READ16(addr) gb_read16(cpu->gb, addr)
#define WRITE8(addr, data) gb_write8(cpu->gb, addr, data)
#define WRITE16(addr, data) gb_write16(cpu->gb, addr, data)

#define PC cpu->pc
#define SP cpu->sp
#define B cpu->b
#define C cpu->c
#define BC cpu->bc
#define D cpu->d
#define E cpu->e
#define DE cpu->de
#define H cpu->h
#define L cpu->l
#define HL cpu->hl
#define A cpu->a
#define F cpu->f
#define AF cpu->af
#define F_Z cpu->f_z
#define F_N cpu->f_n
#define F_H cpu->f_h
#define F_C cpu->f_c
#define F_X cpu->f_x

#define LOWER_NIBBLE (op & 0x0F)
#define UPPER_NIBBLE ((op & 0xF0) >> 4)


struct instr_t instructions[512] = {
    // 0x0X
    {&nop,  1, "NOP"},
    {&ld,   3, "LD BC, "},
    {&ld,   1, "LD (BC), A"},
    {&inc,  1, "INC BC"},
    {&inc,  1, "INC B"},
    {&dec,  1, "DEC B"},
    {&ld,   2, "LD B, "},
    {&rlca, 1, "RLCA"},
    {&ld,   3, "LD (xxxx), SP  "},
    {&add,  1, "ADD HL, BC"},
    {&ld,   1, "LD A, (BC)"},
    {&dec,  1, "DEC BC"},
    {&inc,  1, "INC C"},
    {&dec,  1, "DEC C"},
    {&ld,   2, "LD C, "},
    {&rrca, 1, "RRCA"},
    // 0x1X
    {&stop, 2, "STOP 0"},
    {&ld,   3, "LD DE, "},
    {&ld,   1, "LD (DE), A"},
    {&inc,  1, "INC DE"},
    {&inc,  1, "INC D"},
    {&dec,  1, "DEC D"},
    {&ld,   2, "LD D, "},
    {&rla,  1, "RLA"},
    {&jr,   2, "JR "},
    {&add,  1, "ADD HL, DE"},
    {&ld,   1, "LD A, (DE)"},
    {&dec,  1, "DEC DE"},
    {&inc,  1, "INC E"},
    {&dec,  1, "DEC E"},
    {&ld,   2, "LD E, "},
    {&rra,  1, "RRA"},
    // 0x2X
    {&jr,   2, "JR NZ, "},
    {&ld,   3, "LD HL, "},
    {&ldi,  1, "LDI (HL), A"},
    {&inc,  1, "INC HL"},
    {&inc,  1, "INC H"},
    {&dec,  1, "DEC H"},
    {&ld,   2, "LD H, "},
    {&daa,  1, "DAA"},
    {&jr,   2, "JR Z, "},
    {&add,  1, "ADD HL, HL"},
    {&ldi,  1, "LDI A, (HL)"},
    {&dec,  1, "DEC HL"},
    {&inc,  1, "INC L"},
    {&dec,  1, "DEC L"},
    {&ld,   2, "LD L, "},
    {&cpl,  1, "CPL"},
    // 0x3X
    {&jr,   2, "JR NC, "},
    {&ld,   3, "LD SP, "},
    {&ldd,  1, "LDD (HL), A"},
    {&inc,  1, "INC SP"},
    {&inc,  1, "INC (HL)"},
    {&dec,  1, "DEC (HL)"},
    {&ld,   2, "LD (HL), "},
    {&scf,  1, "SCF"},
    {&jr,   2, "JR C, "},
    {&add,  1, "ADD HL, SP"},
    {&ldd,  1, "LDD A, (HL)"},
    {&dec,  1, "DEC SP"},
    {&inc,  1, "INC A"},
    {&dec,  1, "DEC A"},
    {&ld,   2, "LD A, "},
    {&ccf,  1, "CCF"},
    // 0x4X
    {&ld,   1, "LD B, B"},
    {&ld,   1, "LD B, C"},
    {&ld,   1, "LD B, D"},
    {&ld,   1, "LD B, E"},
    {&ld,   1, "LD B, H"},
    {&ld,   1, "LD B, L"},
    {&ld,   1, "LD B, (HL)"},
    {&ld,   1, "LD B, A"},
    {&ld,   1, "LD C, B"},
    {&ld,   1, "LD C, C"},
    {&ld,   1, "LD C, D"},
    {&ld,   1, "LD C, E"},
    {&ld,   1, "LD C, H"},
    {&ld,   1, "LD C, L"},
    {&ld,   1, "LD C, (HL)"},
    {&ld,   1, "LD C, A"},
    //0x5X
    {&ld,   1, "LD D, B"},
    {&ld,   1, "LD D, C"},
    {&ld,   1, "LD D, D"},
    {&ld,   1, "LD D, E"},
    {&ld,   1, "LD D, H"},
    {&ld,   1, "LD D, L"},
    {&ld,   1, "LD D, (HL)"},
    {&ld,   1, "LD D, A"},
    {&ld,   1, "LD E, B"},
    {&ld,   1, "LD E, C"},
    {&ld,   1, "LD E, D"},
    {&ld,   1, "LD E, E"},
    {&ld,   1, "LD E, H"},
    {&ld,   1, "LD E, L"},
    {&ld,   1, "LD E, (HL)"},
    {&ld,   1, "LD E, A"},
    // 0x6X
    {&ld,   1, "LD H, B"},
    {&ld,   1, "LD H, C"},
    {&ld,   1, "LD H, D"},
    {&ld,   1, "LD H, E"},
    {&ld,   1, "LD H, H"},
    {&ld,   1, "LD H, L"},
    {&ld,   1, "LD H, (HL)"},
    {&ld,   1, "LD H, A"},
    {&ld,   1, "LD L, B"},
    {&ld,   1, "LD L, C"},
    {&ld,   1, "LD L, D"},
    {&ld,   1, "LD L, E"},
    {&ld,   1, "LD L, H"},
    {&ld,   1, "LD L, L"},
    {&ld,   1, "LD L, (HL)"},
    {&ld,   1, "LD L, A"},
    // 0x7X
    {&ld,   1, "LD (HL), B"},
    {&ld,   1, "LD (HL), C"},
    {&ld,   1, "LD (HL), D"},
    {&ld,   1, "LD (HL), E"},
    {&ld,   1, "LD (HL), H"},
    {&ld,   1, "LD (HL), L"},
    {&halt, 1, "HALT"},
    {&ld,   1, "LD (HL), A"},
    {&ld,   1, "LD A, B"},
    {&ld,   1, "LD A, C"},
    {&ld,   1, "LD A, D"},
    {&ld,   1, "LD A, E"},
    {&ld,   1, "LD A, H"},
    {&ld,   1, "LD A, L"},
    {&ld,   1, "LD A, (HL)"},
    {&ld,   1, "LD A, A"},
    // 0x8X
    {&add,  1, "ADD A, B"},
    {&add,  1, "ADD A, C"},
    {&add,  1, "ADD A, D"},
    {&add,  1, "ADD A, E"},
    {&add,  1, "ADD A, H"},
    {&add,  1, "ADD A, L"},
    {&add,  1, "ADD A, (HL)"},
    {&add,  1, "ADD A, A"},
    {&adc,  1, "ADC A, B"},
    {&adc,  1, "ADC A, C"},
    {&adc,  1, "ADC A, D"},
    {&adc,  1, "ADC A, E"},
    {&adc,  1, "ADC A, H"},
    {&adc,  1, "ADC A, L"},
    {&adc,  1, "ADC A, (HL)"},
    {&adc,  1, "ADC A, A"},
    // 0x9X
    {&sub,  1, "SUB A, B"},
    {&sub,  1, "SUB A, C"},
    {&sub,  1, "SUB A, D"},
    {&sub,  1, "SUB A, E"},
    {&sub,  1, "SUB A, H"},
    {&sub,  1, "SUB A, L"},
    {&sub,  1, "SUB A, (HL)"},
    {&sub,  1, "SUB A, A"},
    {&sbc,  1, "SBC A, B"},
    {&sbc,  1, "SBC A, C"},
    {&sbc,  1, "SBC A, D"},
    {&sbc,  1, "SBC A, E"},
    {&sbc,  1, "SBC A, H"},
    {&sbc,  1, "SBC A, L"},
    {&sbc,  1, "SBC A, (HL)"},
    {&sbc,  1, "SBC A, A"},
    // 0xAX
    {&_and, 1, "AND A, B"},
    {&_and, 1, "AND A, C"},
    {&_and, 1, "AND A, D"},
    {&_and, 1, "AND A, E"},
    {&_and, 1, "AND A, H"},
    {&_and, 1, "AND A, L"},
    {&_and, 1, "AND A, (HL)"},
    {&_and, 1, "AND A, A"},
    {&_xor, 1, "XOR A, B"},
    {&_xor, 1, "XOR A, C"},
    {&_xor, 1, "XOR A, D"},
    {&_xor, 1, "XOR A, E"},
    {&_xor, 1, "XOR A, H"},
    {&_xor, 1, "XOR A, L"},
    {&_xor, 1, "XOR A, (HL)"},
    {&_xor, 1, "XOR A, A"},
    // 0xBX
    {&_or,  1, "OR A, B"},
    {&_or,  1, "OR A, C"},
    {&_or,  1, "OR A, D"},
    {&_or,  1, "OR A, E"},
    {&_or,  1, "OR A, H"},
    {&_or,  1, "OR A, L"},
    {&_or,  1, "OR A, (HL)"},
    {&_or,  1, "OR A, A"},
    {&cp,   1, "CP A, B"},
    {&cp,   1, "CP A, C"},
    {&cp,   1, "CP A, D"},
    {&cp,   1, "CP A, E"},
    {&cp,   1, "CP A, H"},
    {&cp,   1, "CP A, L"},
    {&cp,   1, "CP A, (HL)"},
    {&cp,   1, "CP A, A"},
    // 0xCX
    {&ret,  1, "RET NZ"},
    {&pop,  1, "POP BC"},
    {&jp,   3, "JP NZ, "},
    {&jp,   3, "JP "},
    {&call, 3, "CALL NZ, "},
    {&push, 1, "PUSH BC"},
    {&add,  2, "ADD A, "},
    {&rst,  1, "RST 00H"},
    {&ret,  1, "RET Z"},
    {&ret,  1, "RET"},
    {&jp,   3, "JP Z, "},
    {&cb,   1, "PREFIX CB"},
    {&call, 3, "CALL Z, "},
    {&call, 3, "CALL "},
    {&adc,  2, "ADC A, "},
    {&rst,  1, "RST 08H"},
    // 0xDX
    {&ret,  1, "RET NC"},
    {&pop,  1, "POP DE"},
    {&jp,   3, "JP NC, "},
    {&und,  1, "UNDEFINED"},
    {&call, 3, "CALL NC, "},
    {&push, 1, "PUSH DE"},
    {&sub,  2, "SUB A, "},
    {&rst,  1, "RST 10H"},
    {&ret,  1, "RET C"},
    {&reti, 1, "RETI"},
    {&jp,   3, "JP C, "},
    {&und,  1, "UNDEFINED"},
    {&call, 3, "CALL C, "},
    {&und,  1, "UNDEFINED"},
    {&sbc,  2, "SBC A, "},
    {&rst,  1, "RST 18H"},
    // 0xEX
    {&ldh,  2, "LDH (FFxx), A  "},
    {&pop,  1, "POP HL"},
    {&ldh,  1, "LDH (FF00+C), A"},
    {&und,  1, "UNDEFINED"},
    {&und,  1, "UNDEFINED"},
    {&push, 1, "PUSH HL"},
    {&_and, 2, "AND A, "},
    {&rst,  1, "RST 20H"},
    {&add,  2, "ADD SP, "},
    {&jp,   1, "JP HL"},
    {&ld,   3, "LD (xxxx), A  "},
    {&und,  1, "UNDEFINED"},
    {&und,  1, "UNDEFINED"},
    {&und,  1, "UNDEFINED"},
    {&_xor, 2, "XOR A, "},
    {&rst,  1, "RST 28H"},
    // 0xFX
    {&ldh,  2, "LDH A, (FFxx)  "},
    {&pop,  1, "POP AF"},
    {&ldh,  1, "LDH A, (FF00+C)"},
    {&di,   1, "DI"},
    {&und,  1, "UNDEFINED"},
    {&push, 1, "PUSH AF"},
    {&_or,  2, "OR A, "},
    {&rst,  1, "RST 30H"},
    {&ld,   2, "LD HL, SP+"},
    {&ld,   1, "LD SP, HL"},
    {&ld,   3, "LD A, (xxxx)  "},
    {&ei,   1, "EI"},
    {&und,  1, "UNDEFINED"},
    {&und,  1, "UNDEFINED"},
    {&cp,   2, "CP A, "},
    {&rst,  1, "RST 38H"},

    // CB Opcodes
    // 0x0X
    {&rlc,  1, "RLC B"},
    {&rlc,  1, "RLC C"},
    {&rlc,  1, "RLC D"},
    {&rlc,  1, "RLC E"},
    {&rlc,  1, "RLC H"},
    {&rlc,  1, "RLC L"},
    {&rlc,  1, "RLC (HL)"},
    {&rlc,  1, "RLC A"},
    {&rrc,  1, "RRC B"},
    {&rrc,  1, "RRC C"},
    {&rrc,  1, "RRC D"},
    {&rrc,  1, "RRC E"},
    {&rrc,  1, "RRC H"},
    {&rrc,  1, "RRC L"},
    {&rrc,  1, "RRC (HL)"},
    {&rrc,  1, "RRC A"},
    // 0x1X
    {&rl,   1, "RL B"},
    {&rl,   1, "RL C"},
    {&rl,   1, "RL D"},
    {&rl,   1, "RL E"},
    {&rl,   1, "RL H"},
    {&rl,   1, "RL L"},
    {&rl,   1, "RL (HL)"},
    {&rl,   1, "RL A"},
    {&rr,   1, "RR B"},
    {&rr,   1, "RR C"},
    {&rr,   1, "RR D"},
    {&rr,   1, "RR E"},
    {&rr,   1, "RR H"},
    {&rr,   1, "RR L"},
    {&rr,   1, "RR (HL)"},
    {&rr,   1, "RR A"},
    // 0x2X
    {&sla,  1, "SLA B"},
    {&sla,  1, "SLA C"},
    {&sla,  1, "SLA D"},
    {&sla,  1, "SLA E"},
    {&sla,  1, "SLA H"},
    {&sla,  1, "SLA L"},
    {&sla,  1, "SLA (HL)"},
    {&sla,  1, "SLA A"},
    {&sra,  1, "SRA B"},
    {&sra,  1, "SRA C"},
    {&sra,  1, "SRA D"},
    {&sra,  1, "SRA E"},
    {&sra,  1, "SRA H"},
    {&sra,  1, "SRA L"},
    {&sra,  1, "SRA (HL)"},
    {&sra,  1, "SRA A"},
    // 0x3X
    {&swap, 1, "SWAP B"},
    {&swap, 1, "SWAP C"},
    {&swap, 1, "SWAP D"},
    {&swap, 1, "SWAP E"},
    {&swap, 1, "SWAP H"},
    {&swap, 1, "SWAP L"},
    {&swap, 1, "SWAP (HL)"},
    {&swap, 1, "SWAP A"},
    {&srl,  1, "SRL B"},
    {&srl,  1, "SRL C"},
    {&srl,  1, "SRL D"},
    {&srl,  1, "SRL E"},
    {&srl,  1, "SRL H"},
    {&srl,  1, "SRL L"},
    {&srl,  1, "SRL (HL)"},
    {&srl,  1, "SRL A"},
    // 0x4X
    {&bit,  1, "BIT 0, B"},
    {&bit,  1, "BIT 0, C"},
    {&bit,  1, "BIT 0, D"},
    {&bit,  1, "BIT 0, E"},
    {&bit,  1, "BIT 0, H"},
    {&bit,  1, "BIT 0, L"},
    {&bit,  1, "BIT 0, (HL)"},
    {&bit,  1, "BIT 0, A"},
    {&bit,  1, "BIT 1, B"},
    {&bit,  1, "BIT 1, C"},
    {&bit,  1, "BIT 1, D"},
    {&bit,  1, "BIT 1, E"},
    {&bit,  1, "BIT 1, H"},
    {&bit,  1, "BIT 1, L"},
    {&bit,  1, "BIT 1, (HL)"},
    {&bit,  1, "BIT 1, A"},
    // 0x5X
    {&bit,  1, "BIT 2, B"},
    {&bit,  1, "BIT 2, C"},
    {&bit,  1, "BIT 2, D"},
    {&bit,  1, "BIT 2, E"},
    {&bit,  1, "BIT 2, H"},
    {&bit,  1, "BIT 2, L"},
    {&bit,  1, "BIT 2, (HL)"},
    {&bit,  1, "BIT 2, A"},
    {&bit,  1, "BIT 3, B"},
    {&bit,  1, "BIT 3, C"},
    {&bit,  1, "BIT 3, D"},
    {&bit,  1, "BIT 3, E"},
    {&bit,  1, "BIT 3, H"},
    {&bit,  1, "BIT 3, L"},
    {&bit,  1, "BIT 3, (HL)"},
    {&bit,  1, "BIT 3, A"},
    // 0x6X
    {&bit,  1, "BIT 4, B"},
    {&bit,  1, "BIT 4, C"},
    {&bit,  1, "BIT 4, D"},
    {&bit,  1, "BIT 4, E"},
    {&bit,  1, "BIT 4, H"},
    {&bit,  1, "BIT 4, L"},
    {&bit,  1, "BIT 4, (HL)"},
    {&bit,  1, "BIT 4, A"},
    {&bit,  1, "BIT 5, B"},
    {&bit,  1, "BIT 5, C"},
    {&bit,  1, "BIT 5, D"},
    {&bit,  1, "BIT 5, E"},
    {&bit,  1, "BIT 5, H"},
    {&bit,  1, "BIT 5, L"},
    {&bit,  1, "BIT 5, (HL)"},
    {&bit,  1, "BIT 5, A"},
    // 0x7X
    {&bit,  1, "BIT 6, B"},
    {&bit,  1, "BIT 6, C"},
    {&bit,  1, "BIT 6, D"},
    {&bit,  1, "BIT 6, E"},
    {&bit,  1, "BIT 6, H"},
    {&bit,  1, "BIT 6, L"},
    {&bit,  1, "BIT 6, (HL)"},
    {&bit,  1, "BIT 6, A"},
    {&bit,  1, "BIT 7, B"},
    {&bit,  1, "BIT 7, C"},
    {&bit,  1, "BIT 7, D"},
    {&bit,  1, "BIT 7, E"},
    {&bit,  1, "BIT 7, H"},
    {&bit,  1, "BIT 7, L"},
    {&bit,  1, "BIT 7, (HL)"},
    {&bit,  1, "BIT 7, A"},
    // 0x8X
    {&res,  1, "RES 0, B"},
    {&res,  1, "RES 0, C"},
    {&res,  1, "RES 0, D"},
    {&res,  1, "RES 0, E"},
    {&res,  1, "RES 0, H"},
    {&res,  1, "RES 0, L"},
    {&res,  1, "RES 0, (HL)"},
    {&res,  1, "RES 0, A"},
    {&res,  1, "RES 1, B"},
    {&res,  1, "RES 1, C"},
    {&res,  1, "RES 1, D"},
    {&res,  1, "RES 1, E"},
    {&res,  1, "RES 1, H"},
    {&res,  1, "RES 1, L"},
    {&res,  1, "RES 1, (HL)"},
    {&res,  1, "RES 1, A"},
    // 0x9X
    {&res,  1, "RES 2, B"},
    {&res,  1, "RES 2, C"},
    {&res,  1, "RES 2, D"},
    {&res,  1, "RES 2, E"},
    {&res,  1, "RES 2, H"},
    {&res,  1, "RES 2, L"},
    {&res,  1, "RES 2, (HL)"},
    {&res,  1, "RES 2, A"},
    {&res,  1, "RES 3, B"},
    {&res,  1, "RES 3, C"},
    {&res,  1, "RES 3, D"},
    {&res,  1, "RES 3, E"},
    {&res,  1, "RES 3, H"},
    {&res,  1, "RES 3, L"},
    {&res,  1, "RES 3, (HL)"},
    {&res,  1, "RES 3, A"},
    // 0xAX
    {&res,  1, "RES 4, B"},
    {&res,  1, "RES 4, C"},
    {&res,  1, "RES 4, D"},
    {&res,  1, "RES 4, E"},
    {&res,  1, "RES 4, H"},
    {&res,  1, "RES 4, L"},
    {&res,  1, "RES 4, (HL)"},
    {&res,  1, "RES 4, A"},
    {&res,  1, "RES 5, B"},
    {&res,  1, "RES 5, C"},
    {&res,  1, "RES 5, D"},
    {&res,  1, "RES 5, E"},
    {&res,  1, "RES 5, H"},
    {&res,  1, "RES 5, L"},
    {&res,  1, "RES 5, (HL)"},
    {&res,  1, "RES 5, A"},
    // 0xBX
    {&res,  1, "RES 6, B"},
    {&res,  1, "RES 6, C"},
    {&res,  1, "RES 6, D"},
    {&res,  1, "RES 6, E"},
    {&res,  1, "RES 6, H"},
    {&res,  1, "RES 6, L"},
    {&res,  1, "RES 6, (HL)"},
    {&res,  1, "RES 6, A"},
    {&res,  1, "RES 7, B"},
    {&res,  1, "RES 7, C"},
    {&res,  1, "RES 7, D"},
    {&res,  1, "RES 7, E"},
    {&res,  1, "RES 7, H"},
    {&res,  1, "RES 7, L"},
    {&res,  1, "RES 7, (HL)"},
    {&res,  1, "RES 7, A"},
    // 0xCX
    {&set,  1, "SET 0, B"},
    {&set,  1, "SET 0, C"},
    {&set,  1, "SET 0, D"},
    {&set,  1, "SET 0, E"},
    {&set,  1, "SET 0, H"},
    {&set,  1, "SET 0, L"},
    {&set,  1, "SET 0, (HL)"},
    {&set,  1, "SET 0, A"},
    {&set,  1, "SET 1, B"},
    {&set,  1, "SET 1, C"},
    {&set,  1, "SET 1, D"},
    {&set,  1, "SET 1, E"},
    {&set,  1, "SET 1, H"},
    {&set,  1, "SET 1, L"},
    {&set,  1, "SET 1, (HL)"},
    {&set,  1, "SET 1, A"},
    // 0xDX
    {&set,  1, "SET 2, B"},
    {&set,  1, "SET 2, C"},
    {&set,  1, "SET 2, D"},
    {&set,  1, "SET 2, E"},
    {&set,  1, "SET 2, H"},
    {&set,  1, "SET 2, L"},
    {&set,  1, "SET 2, (HL)"},
    {&set,  1, "SET 2, A"},
    {&set,  1, "SET 3, B"},
    {&set,  1, "SET 3, C"},
    {&set,  1, "SET 3, D"},
    {&set,  1, "SET 3, E"},
    {&set,  1, "SET 3, H"},
    {&set,  1, "SET 3, L"},
    {&set,  1, "SET 3, (HL)"},
    {&set,  1, "SET 3, A"},
    // 0xEX
    {&set,  1, "SET 4, B"},
    {&set,  1, "SET 4, C"},
    {&set,  1, "SET 4, D"},
    {&set,  1, "SET 4, E"},
    {&set,  1, "SET 4, H"},
    {&set,  1, "SET 4, L"},
    {&set,  1, "SET 4, (HL)"},
    {&set,  1, "SET 4, A"},
    {&set,  1, "SET 5, B"},
    {&set,  1, "SET 5, C"},
    {&set,  1, "SET 5, D"},
    {&set,  1, "SET 5, E"},
    {&set,  1, "SET 5, H"},
    {&set,  1, "SET 5, L"},
    {&set,  1, "SET 5, (HL)"},
    {&set,  1, "SET 5, A"},
    // 0xFX
    {&set,  1, "SET 6, B"},
    {&set,  1, "SET 6, C"},
    {&set,  1, "SET 6, D"},
    {&set,  1, "SET 6, E"},
    {&set,  1, "SET 6, H"},
    {&set,  1, "SET 6, L"},
    {&set,  1, "SET 6, (HL)"},
    {&set,  1, "SET 6, A"},
    {&set,  1, "SET 7, B"},
    {&set,  1, "SET 7, C"},
    {&set,  1, "SET 7, D"},
    {&set,  1, "SET 7, E"},
    {&set,  1, "SET 7, H"},
    {&set,  1, "SET 7, L"},
    {&set,  1, "SET 7, (HL)"},
    {&set,  1, "SET 7, A"}
};


int invalid_instr(u8 op) {
    printf("Warning: Invalid instruction called for opcode %#02x!\n", op);
    return -1;
}

int und(struct cpu* cpu, u8 op) {
    printf("Warning: Invalid opcode: %#02x!\n", op);
    return -1;
}
int nop(struct cpu* cpu, u8 op) {
    return 4;
}
int stop(struct cpu* cpu, u8 op) {
    /*TODO: Implement */
    return 4;
}
int halt(struct cpu* cpu, u8 op) {
    /*TODO: Implement */
    return 4;
}
int ei(struct cpu* cpu, u8 op) {
    cpu->imeWait = 1;
    return 4;
}
int di(struct cpu* cpu, u8 op) {
    cpu->ime = false;
    return 4;
}
int cb(struct cpu* cpu, u8 op) {
    u8 nextop = READ8(PC++);

    int (*instr_ptr)(struct cpu*, u8) = instructions[nextop + 256].ptr;
    return instr_ptr(cpu, nextop) + 4; /* TODO: Do each of the CB instructions include this 4? */
}

int inc(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x34) {
        u8 v = READ8(HL);
        F_H = (((v & 0xF) + 1) & 0x10) == 0x10;
        v++;
        F_Z = (v == 0);
        F_N = 0;
        WRITE8(HL, v);
        return 12;
    }
    // r16
    else if(LOWER_NIBBLE == 0x3) {
        u16* r16;
        if(UPPER_NIBBLE == 0x0)      r16 = &BC;
        else if(UPPER_NIBBLE == 0x1) r16 = &DE;
        else if(UPPER_NIBBLE == 0x2) r16 = &HL;
        else if(UPPER_NIBBLE == 0x3) r16 = &SP;
        else return invalid_instr(op);

        (*r16)++;
        return 8;
    }
    // r8
    else if(LOWER_NIBBLE == 0x4 || LOWER_NIBBLE == 0xC) {
        u8* r8;
        if(LOWER_NIBBLE == 0x4) {
            if(UPPER_NIBBLE == 0x0)      r8 = &B;
            else if(UPPER_NIBBLE == 0x1) r8 = &D;
            else if(UPPER_NIBBLE == 0x2) r8 = &H;
            else return invalid_instr(op);
        }
        else if(LOWER_NIBBLE == 0xC) {
            if(UPPER_NIBBLE == 0x0)      r8 = &C;
            else if(UPPER_NIBBLE == 0x1) r8 = &E;
            else if(UPPER_NIBBLE == 0x2) r8 = &L;
            else if(UPPER_NIBBLE == 0x3) r8 = &A;
            else return invalid_instr(op);
        }
        F_H = (((*r8 & 0xF) + 1) & 0x10) == 0x10;
        (*r8)++;
        F_Z = (*r8 == 0);
        F_N = 0;
        return 4;
    }

    return invalid_instr(op);
}

int dec(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x35) {
        u8 v = READ8(HL);
        F_H = ((v & 0xF) < 1);
        v--;
        F_Z = (v == 0);
        F_N = 1;
        WRITE8(HL, v);
        return 12;
    }
    // r16
    else if(LOWER_NIBBLE == 0xB) {
        u16* r16;
        if(UPPER_NIBBLE == 0x0)      r16 = &BC;
        else if(UPPER_NIBBLE == 0x1) r16 = &DE;
        else if(UPPER_NIBBLE == 0x2) r16 = &HL;
        else if(UPPER_NIBBLE == 0x3) r16 = &SP;
        else return invalid_instr(op);

        (*r16)--;
        return 8;
    }
    // r8
    else if(LOWER_NIBBLE == 0x5 || LOWER_NIBBLE == 0xD) {
        u8* r8;
        if(LOWER_NIBBLE == 0x5) {
            if(UPPER_NIBBLE == 0x0)      r8 = &B;
            else if(UPPER_NIBBLE == 0x1) r8 = &D;
            else if(UPPER_NIBBLE == 0x2) r8 = &H;
            else return invalid_instr(op);
        }
        else if(LOWER_NIBBLE == 0xD) {
            if(UPPER_NIBBLE == 0x0)      r8 = &C;
            else if(UPPER_NIBBLE == 0x1) r8 = &E;
            else if(UPPER_NIBBLE == 0x2) r8 = &L;
            else if(UPPER_NIBBLE == 0x3) r8 = &A;
            else return invalid_instr(op);
        }
        F_H = (*r8 & 0xF) < 1;
        (*r8)--;
        F_Z = (*r8 == 0);
        F_N = 1;
        return 4;
    }

    return invalid_instr(op);
}

int add(struct cpu* cpu, u8 op) {
    // SP, r8
    if(op == 0xE8) {
        s8 s = (s8) READ8(PC++);
        F_C = ((SP + s) & 0x10000) == 0x10000;
        F_H = (((SP & 0xFFF) + s) & 0x1000) == 0x1000;
        SP += s;
        F_Z = 0;
        F_N = 0;
        return 16;
    }
    // A, d8
    else if(op == 0xC6) {
        u8 v = READ8(PC++);
        F_C = ((A + v) & 0x100) == 0x100;
        F_H = (((A & 0xF) + (v & 0xF)) & 0x10) == 0x10;
        A += v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // HL, r16
    else if(LOWER_NIBBLE == 0x9) {
        u16* r16;
        if(UPPER_NIBBLE == 0x0)      r16 = &BC;
        else if(UPPER_NIBBLE == 0x1) r16 = &DE;
        else if(UPPER_NIBBLE == 0x2) r16 = &HL;
        else if(UPPER_NIBBLE == 0x3) r16 = &SP;
        else return invalid_instr(op);
        
        F_C = ((HL + *r16) & 0x10000) == 0x10000;
        F_H = (((HL & 0xFFF) + (*r16 & 0xFFF)) & 0x1000) == 0x1000;
        HL += *r16;
        F_N = 0;
        return 8;
    }
    // A, (HL)
    else if(op == 0x86) {
        u8 v = READ8(HL);
        F_C = ((A + v) & 0x100) == 0x100;
        F_H = (((A & 0xF) + (v & 0xF)) & 0x10) == 0x10;
        A += v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 0x8) {
        u8* r8;
        if(LOWER_NIBBLE == 0)      r8 = &B;
        else if(LOWER_NIBBLE == 1) r8 = &C;
        else if(LOWER_NIBBLE == 2) r8 = &D;
        else if(LOWER_NIBBLE == 3) r8 = &E;
        else if(LOWER_NIBBLE == 4) r8 = &H;
        else if(LOWER_NIBBLE == 5) r8 = &L;
        else if(LOWER_NIBBLE == 7) r8 = &A;
        else return invalid_instr(op);

        F_C = ((A + *r8) & 0x100) == 0x100;
        F_H = (((A & 0xF) + (*r8 & 0xF)) & 0x10) == 0x10;
        A += *r8;
        F_Z = (A == 0);
        F_N = 0;
        return 4;
    }

    return invalid_instr(op);
}

int adc(struct cpu* cpu, u8 op) {
    // A, d8
    if(op == 0xCE) {
        u8 oc = F_C;
        u8 v = READ8(PC++);
        F_C = ((A + v + oc) & 0x100) == 0x100;
        F_H = (((A & 0xF) + (v & 0xF) + oc) & 0x10) == 0x10;
        A += v + oc;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // A, (HL)
    else if(op == 0x8E) {
        u8 oc = F_C;
        u8 v = READ8(HL);
        F_C = ((A + v + oc) & 0x100) == 0x100;
        F_H = (((A & 0xF) + (v & 0xF) + oc) & 0x10) == 0x10;
        A += v + oc;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 8) {
        u8* r8;

        if(LOWER_NIBBLE == 8)        r8 = &B;
        else if(LOWER_NIBBLE == 9)   r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);

        u8 oc = F_C;
        F_C = ((A + *r8 + oc) & 0x100) == 0x100;
        F_H = (((A & 0xF) + (*r8 & 0xF) + oc) & 0x10) == 0x10;
        A += *r8 + oc;
        F_Z = (A == 0);
        F_N = 0;
        return 4;
    }

    return invalid_instr(op);
}

int sub(struct cpu* cpu, u8 op) {
    // A, d8
    if(op == 0xD6) {
        u8 v = READ8(PC++);
        F_C = (A < v);
        F_H = ((A & 0xF) < (v & 0xF));
        A -= v;
        F_Z = (A == 0);
        F_N = 1;
        return 8;
    }
    // A, (HL)
    else if(op == 0x96) {
        u8 v = READ8(HL);
        F_C = (A < v);
        F_H = ((A & 0xF) < (v & 0xF));
        A -= v;
        F_Z = (A == 0);
        F_N = 1;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 9) {
        u8* r8;

        if(LOWER_NIBBLE == 0)      r8 = &B;
        else if(LOWER_NIBBLE == 1) r8 = &C;
        else if(LOWER_NIBBLE == 2) r8 = &D;
        else if(LOWER_NIBBLE == 3) r8 = &E;
        else if(LOWER_NIBBLE == 4) r8 = &H;
        else if(LOWER_NIBBLE == 5) r8 = &L;
        else if(LOWER_NIBBLE == 7) r8 = &A;
        else return invalid_instr(op);

        F_C = (A < *r8);
        F_H = ((A & 0xF) < (*r8 & 0xF));
        A -= *r8;
        F_Z = (A == 0);
        F_N = 1;
        return 4;
    }

    return invalid_instr(op);
}

int sbc(struct cpu* cpu, u8 op) {
    // A, d8
    if(op == 0xDE) {
        u8 v = READ8(PC++);
        u8 oc = F_C;
        F_C = (A < (v + oc));
        F_H = ((A & 0xF) < ((v & 0xF) + oc));
        A -= (v + oc);
        F_Z = (A == 0);
        F_N = 1;
        return 8;
    }
    // A, (HL)
    else if(op == 0x9E) {
        u8 v = READ8(HL);
        u8 oc = F_C;
        F_C = (A < (v + oc));
        F_H = ((A & 0xF) < ((v & 0xF) + oc));
        A -= (v + oc);
        F_Z = (A == 0);
        F_N = 1;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 9) {
        u8* r8;

        if(LOWER_NIBBLE == 0x8)      r8 = &B;
        else if(LOWER_NIBBLE == 0x9) r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);
        
        u8 oc = F_C;
        F_C = (A < (*r8 + oc));
        F_H = ((A & 0xF) < ((*r8 & 0xF) + oc));
        A -= (*r8 + oc);
        F_Z = (A == 0);
        F_N = 1;
        return 4;
    }

    return invalid_instr(op);
}

int ld(struct cpu* cpu, u8 op) {
    if(UPPER_NIBBLE <= 0x3) {
        // (a16), SP
        if(op == 0x08) {
            WRITE16(READ16(PC), SP);
            PC += 2;
            return 20;
        }
        // r16, d16
        if(LOWER_NIBBLE == 0x1) {
            u16* r16;
            if(UPPER_NIBBLE == 0)      r16 = &BC;
            else if(UPPER_NIBBLE == 1) r16 = &DE;
            else if(UPPER_NIBBLE == 2) r16 = &HL;
            else if(UPPER_NIBBLE == 3) r16 = &SP;
            else return invalid_instr(op);

            *r16 = READ16(PC);
            PC += 2;
            return 12;
        }
        // (HL), d8
        else if(op == 0x36) {
            WRITE8(HL, READ8(PC++));
            return 12;
        }
        // r8, d8
        else if(LOWER_NIBBLE == 0x6 || LOWER_NIBBLE == 0xE) {
            u8* r8;
            if(LOWER_NIBBLE == 0x6) {
                if(UPPER_NIBBLE == 0)      r8 = &B;
                else if(UPPER_NIBBLE == 1) r8 = &D;
                else if(UPPER_NIBBLE == 2) r8 = &H;
                else return invalid_instr(op);
            }
            else if(LOWER_NIBBLE == 0xE) {
                if(UPPER_NIBBLE == 0)      r8 = &C;
                else if(UPPER_NIBBLE == 1) r8 = &E;
                else if(UPPER_NIBBLE == 2) r8 = &L;
                else if(UPPER_NIBBLE == 3) r8 = &A;
                else return invalid_instr(op);
            }
            *r8 = READ8(PC++);
            return 8;
        }
        else if(UPPER_NIBBLE <= 0x1) {
            // (r16), A
            if(LOWER_NIBBLE == 0x2) {
                u16* r16;
                if(UPPER_NIBBLE == 0x0)       r16 = &BC;
                else if(UPPER_NIBBLE == 0x1)  r16 = &DE;
                else return invalid_instr(op);
                
                WRITE8(*r16, A);
                return 8;
            }
            // A, (r16)
            else if(LOWER_NIBBLE == 0xA) {
                u16* r16;
                if(UPPER_NIBBLE == 0x0)       r16 = &BC;
                else if(UPPER_NIBBLE == 0x1)  r16 = &DE;
                else return invalid_instr(op);

                A = READ8(*r16);
                return 8;
            }
        }
    }
    else if(UPPER_NIBBLE >= 4 && UPPER_NIBBLE <= 7) {
        // (HL), r8
        if(UPPER_NIBBLE == 7 && LOWER_NIBBLE <= 7) {
            u8* r8;
            if(LOWER_NIBBLE == 0x0)      r8 = &B;
            else if(LOWER_NIBBLE == 0x1) r8 = &C;
            else if(LOWER_NIBBLE == 0x2) r8 = &D;
            else if(LOWER_NIBBLE == 0x3) r8 = &E;
            else if(LOWER_NIBBLE == 0x4) r8 = &H;
            else if(LOWER_NIBBLE == 0x5) r8 = &L;
            else if(LOWER_NIBBLE == 0x7) r8 = &A;
            else return invalid_instr(op);
            
            WRITE8(HL, *r8);
            return 8;
        }
        // r8, (HL)
        else if(LOWER_NIBBLE == 0x6 || LOWER_NIBBLE == 0xE) {
            u8* r8;
            if(LOWER_NIBBLE == 6) {
                if(UPPER_NIBBLE == 0x4)      r8 = &B;
                else if(UPPER_NIBBLE == 0x5) r8 = &D;
                else if(UPPER_NIBBLE == 0x6) r8 = &H;
                else return invalid_instr(op);
            }
            else if(LOWER_NIBBLE == 0xE) {
                if(UPPER_NIBBLE == 0x4)      r8 = &C;
                else if(UPPER_NIBBLE == 0x5) r8 = &E;
                else if(UPPER_NIBBLE == 0x6) r8 = &L;
                else if(UPPER_NIBBLE == 0x7) r8 = &A;
                else return invalid_instr(op);
            }
            *r8 = READ8(HL);
            return 8;
        }
        // r8, r8
        else if(op != 0x76) {
            u8* dest;
            u8* src;
            if(LOWER_NIBBLE == 0x0 || LOWER_NIBBLE == 0x8)      src = &B;
            else if(LOWER_NIBBLE == 0x1 || LOWER_NIBBLE == 0x9) src = &C;
            else if(LOWER_NIBBLE == 0x2 || LOWER_NIBBLE == 0xA) src = &D;
            else if(LOWER_NIBBLE == 0x3 || LOWER_NIBBLE == 0xB) src = &E;
            else if(LOWER_NIBBLE == 0x4 || LOWER_NIBBLE == 0xC) src = &H;
            else if(LOWER_NIBBLE == 0x5 || LOWER_NIBBLE == 0xD) src = &L;
            else if(LOWER_NIBBLE == 0x7 || LOWER_NIBBLE == 0xF) src = &A;
            else return invalid_instr(op);

            if(UPPER_NIBBLE == 4) {
                if(LOWER_NIBBLE <= 7) dest = &B;
                else                  dest = &C;
            }
            else if(UPPER_NIBBLE == 5) {
                if(LOWER_NIBBLE <= 7) dest = &D;
                else                  dest = &E;
            }
            else if(UPPER_NIBBLE == 6) {
                if(LOWER_NIBBLE <= 7) dest = &H;
                else                  dest = &L;
            }
            else if(UPPER_NIBBLE == 7) {
                if(LOWER_NIBBLE >= 8) dest = &A;
                else                  return invalid_instr(op);
            }
            else return invalid_instr(op);

            *dest = *src;
            return 4;
        }
        return invalid_instr(op);
    }
    // SP, HL
    else if(op == 0xF9) {
        SP = HL;
        return 8;
    }
    // HL, SP+r8
    else if(op == 0xF8) {
        s8 s = (s8) READ8(PC++);
        F_C = ((SP + s) & 0x10000) == 0x10000;
        F_H = (((SP & 0xFFF) + s) & 0x1000) == 0x1000;
        HL = SP + s;
        F_Z = 0;
        F_N = 0;
        return 12;
    }
    // (a16), A
    else if(op == 0xEA) {
        WRITE8(READ16(PC), A);
        PC += 2;
        return 16;
    }
    // A, (a16)
    else if(op == 0xFA) {
        A = READ8(READ16(PC));
        PC += 2;
        return 16;
    }

    return invalid_instr(op);
}

int ldh(struct cpu* cpu, u8 op) {
    // (a8), A
    if(op == 0xE0) {
        WRITE8(0xFF00 + READ8(PC++), A);
        return 12;
    }
    // A, (a8)
    else if(op == 0xF0) {
        A = READ8(0xFF00 + READ8(PC++));
        return 12;
    }
    // (C), A
    else if(op == 0xE2) {
        WRITE8(0xFF00 + C, A);
        return 8;
    }
    // A, (C)
    else if(op == 0xF2) {
        A = READ8(0xFF00 + C);
        return 8;
    }
    
    return invalid_instr(op);
}

int ldi(struct cpu* cpu, u8 op) {
    // (HL+), A
    if(op == 0x22) {
        WRITE8(HL, A);
        HL++;
        return 8;
    }
    // A, (HL+)
    else if(op == 0x2A) {
        A = READ8(HL);
        HL++;
        return 8;
    }

    return invalid_instr(op);
}

int ldd(struct cpu* cpu, u8 op) {
    // (HL-), A
    if(op == 0x32) {
        WRITE8(HL, A);
        HL--;
        return 8;
    }
    // A, (HL-)
    else if(op == 0x3A) {
        A = READ8(HL);
        HL--;
        return 8;
    }

    return invalid_instr(op);
}

int jp(struct cpu* cpu, u8 op) {
    // HL
    if(op == 0xE9) {
        PC = HL;
        return 4;
    }

    bool cond = false;
    // a16
    if(op == 0xC3) cond = true;
    else if(op == 0xCA) cond = F_Z;
    else if(op == 0xDA) cond = F_C;
    else if(op == 0xC2) cond = !F_Z;
    else if(op == 0xD2) cond = !F_C;
    else return invalid_instr(op);

    if(cond) {
        PC = READ16(PC);
        return 16;
    }
    else {
        PC += 2;
        return 12;
    }
    
    return invalid_instr(op);
}

int jr(struct cpu* cpu, u8 op) {
    bool cond = false;

    if(op == 0x18) cond = true;
    else if(op == 0x28) cond = F_Z;
    else if(op == 0x38) cond = F_C;
    else if(op == 0x20) cond = !F_Z;
    else if(op == 0x30) cond = !F_C;
    else return invalid_instr(op);
    
    if(cond) {
        s8 ofs = READ8(PC++);
        PC += ofs;
        return 12;
    }
    else {
        PC++;
        return 8;
    }

    return invalid_instr(op);
}

int call(struct cpu* cpu, u8 op) {
    bool cond = false;

    if(op == 0xCD) cond = true;
    else if(op == 0xCC) cond = F_Z;
    else if(op == 0xDC) cond = F_C;
    else if(op == 0xC4) cond = !F_Z;
    else if(op == 0xD4) cond = !F_C;
    else return invalid_instr(op);

    if(cond) {
        u16 nextInstrAddr = PC + 2;
        WRITE8(--SP, (nextInstrAddr & 0xFF00) >> 8);
        WRITE8(--SP, nextInstrAddr & 0xFF);
        u16 addr = READ16(PC);
        PC = addr;
        return 24;
    }
    else {
        PC += 2;
        return 12;
    }

    return invalid_instr(op);
}

int ret(struct cpu* cpu, u8 op) {
    bool cond = false;
    
    if(op == 0xC9) cond = true;
    else if(op == 0xC8) cond = F_Z;
    else if(op == 0xD8) cond = F_C;
    else if(op == 0xC0) cond = !F_Z;
    else if(op == 0xD0) cond = !F_C;
    else return invalid_instr(op);

    if(cond) {
        PC = (READ8(SP) | (READ8(SP + 1) << 8));
        SP += 2;
        return (op == 0xC9)? 16 : 20;
    } else {
        return 8;
    }

    return invalid_instr(op);
}

int reti(struct cpu* cpu, u8 op) {
    PC = (READ8(SP) | (READ8(SP + 1) << 8));
    SP += 2;
    cpu->imeWait = 1; // TODO: Should RETI wait or set ime immediately?
    return 16;
}

int push(struct cpu* cpu, u8 op) {
    u16* r16;
    if(op == 0xC5) r16 = &BC;
    else if(op == 0xD5) r16 = &DE;
    else if(op == 0xE5) r16 = &HL;
    else if(op == 0xF5) r16 = &AF;
    else return invalid_instr(op);

    WRITE8(--SP, (*r16) >> 8);
    WRITE8(--SP, (*r16) & 0xFF);
    return 16;
}

int pop(struct cpu* cpu, u8 op) {
    u16* r16;
    if(op == 0xC1) r16 = &BC;
    else if(op == 0xD1) r16 = &DE;
    else if(op == 0xE1) r16 = &HL;
    else if(op == 0xF1) r16 = &AF;
    else return invalid_instr(op);

    *r16 = (READ8(SP) | (READ8(SP + 1) << 8));    
    if(op == 0xF1) {
        // Bottom four bits of F must be 0
        F_X = 0x0;
    }
    SP += 2;

    return 12;
}

int rst(struct cpu* cpu, u8 op) {
    // Encoding is 00xxx000, where xxx is multiplied by 8 for the zero-page offset we jump to
    u8 offset = ((op & 0x38) >> 3) & 0x7;
    offset *= 8;

    WRITE8(--SP, (PC & 0xFF00) >> 8);
    WRITE8(--SP, PC & 0xFF);
    PC = offset;
    return 16;
}

int scf(struct cpu* cpu, u8 op) {
    F_C = 1;
    F_N = 0;
    F_H = 0;
    return 4;
}
int ccf(struct cpu* cpu, u8 op) {
    F_C = ~F_C;
    F_N = 0;
    F_H = 0;
    return 4;
}

int cpl(struct cpu* cpu, u8 op) {
    A = ~A;
    F_N = 1;
    F_H = 1;
    return 4;
}

int rlca(struct cpu* cpu, u8 op) {
    F_C = (A & 0x80) >> 7;
    A <<= 1;
    A |= F_C;
    F_Z = 0;
    F_N = 0;
    F_H = 0;
    return 4;
}

int rla(struct cpu* cpu, u8 op) {
    u8 oc = F_C;
    F_C = (A & 0x80) >> 7;
    A <<= 1;
    A |= oc;
    F_Z = 0;
    F_N = 0;
    F_H = 0;
    return 4;
}

int rrca(struct cpu* cpu, u8 op) {
    F_C = A & 1;
    A >>= 1;
    A |= (F_C << 7);
    F_Z = 0;
    F_N = 0;
    F_H = 0;
    return 4;
}

int rra(struct cpu* cpu, u8 op) {
    u8 oc = F_C;
    F_C = A & 1;
    A >>= 1;
    A |= (oc << 7);
    F_Z = 0;
    F_N = 0;
    F_H = 0;
    return 4;
}

int _and(struct cpu* cpu, u8 op) {
    // d8
    if(op == 0xE6) {
        u8 v = READ8(PC++);
        F_C = 0;
        F_H = 1;
        A &= v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // (HL)
    else if(op == 0xA6) {
        u8 v = READ8(HL);
        F_C = 0;
        F_H = 1;
        A &= v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 0xA && LOWER_NIBBLE <= 7) {
        u8* r8;
        if(LOWER_NIBBLE == 0x0)      r8 = &B;
        else if(LOWER_NIBBLE == 0x1) r8 = &C;
        else if(LOWER_NIBBLE == 0x2) r8 = &D;
        else if(LOWER_NIBBLE == 0x3) r8 = &E;
        else if(LOWER_NIBBLE == 0x4) r8 = &H;
        else if(LOWER_NIBBLE == 0x5) r8 = &L;
        else if(LOWER_NIBBLE == 0x7) r8 = &A;
        else return invalid_instr(op);

        F_C = 0;
        F_H = 1;
        A &= *r8;
        F_Z = (A == 0);
        F_N = 0;
        return 4;
    }

    return invalid_instr(op);
}

int _xor(struct cpu* cpu, u8 op) {
    // d8
    if(op == 0xEE) {
        u8 v = READ8(PC++);
        F_C = 0;
        F_H = 0;
        A ^= v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // (HL)
    else if(op == 0xAE) {
        u8 v = READ8(HL);
        F_C = 0;
        F_H = 0;
        A ^= v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 0xA && LOWER_NIBBLE >= 8) {
        u8* r8;
        if(LOWER_NIBBLE == 0x8)      r8 = &B;
        else if(LOWER_NIBBLE == 0x9) r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);

        F_C = 0;
        F_H = 0;
        A ^= *r8;
        F_Z = (A == 0);
        F_N = 0;
        return 4;
    }

    return invalid_instr(op);
}

int _or(struct cpu* cpu, u8 op) {
    // d8
    if(op == 0xF6) {
        u8 v = READ8(PC++);
        F_C = 0;
        F_H = 0;
        A |= v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // (HL)
    else if(op == 0xB6) {
        u8 v = READ8(HL);
        F_C = 0;
        F_H = 0;
        A |= v;
        F_Z = (A == 0);
        F_N = 0;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 0xB && LOWER_NIBBLE <= 7) {
        u8* r8;
        if(LOWER_NIBBLE == 0x0)      r8 = &B;
        else if(LOWER_NIBBLE == 0x1) r8 = &C;
        else if(LOWER_NIBBLE == 0x2) r8 = &D;
        else if(LOWER_NIBBLE == 0x3) r8 = &E;
        else if(LOWER_NIBBLE == 0x4) r8 = &H;
        else if(LOWER_NIBBLE == 0x5) r8 = &L;
        else if(LOWER_NIBBLE == 0x7) r8 = &A;
        else return invalid_instr(op);

        F_C = 0;
        F_H = 0;
        A |= *r8;
        F_Z = (A == 0);
        F_N = 0;
        return 4;
    }

    return invalid_instr(op);
}

int cp(struct cpu* cpu, u8 op) {
    // d8
    if(op == 0xFE) {
        u8 v = READ8(PC++);
        F_C = (A < v);
        F_H = ((A & 0xF) < (v & 0xF));
        F_Z = (A - v) == 0;
        F_N = 1;
        return 8;
    }
    // (HL)
    else if(op == 0xBE) {
        u8 v = READ8(HL);
        F_C = (A < v);
        F_H = ((A & 0xF) < (v & 0xF));
        F_Z = (A - v) == 0;
        F_N = 1;
        return 8;
    }
    // A, r8
    else if(UPPER_NIBBLE == 0xB && LOWER_NIBBLE >= 8) {
        u8* r8;
        if(LOWER_NIBBLE == 0x8)      r8 = &B;
        else if(LOWER_NIBBLE == 0x9) r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);

        F_C = (A < *r8);
        F_H = ((A & 0xF) < (*r8 & 0xF));
        F_Z = (A - *r8) == 0;
        F_N = 1;
        return 4;
    }

    return invalid_instr(op);
}

// Decimal adjust
/* If the lower nibbles of the two previously added numbers result in
*  a lower nibble result more than 9 (i.e. the lower nibble is A-F or
*  the half-carry is set meaning it was more than F), then add 6 to
*  shift the extra into the higher nibble.
*  If the whole result is greater than 0x99 or there was a carry from
*  the previous addition, then add 6 to the high nibble (+0x60).
*  If the previous instruction was a subtraction then subtract 0x6/0x60
*  instead of adding. Also, with subtraction we only check for
*  carry/half-carry, and not individual digits being greater than 9,
*  because you can't subtract two BCD numbers and have a number greater
*  than 9 without a borrow having occurred, and the GameBoy does nothing
*  if A has a digit greater than 9 and the subtraction flag is set while
*  neither carry nor half-carry is set.
*/
int daa(struct cpu* cpu, u8 op) {
    u8 ofs = 0;
    u8 carry = 0;
    if(F_H || (!F_N && ((A & 0xF) > 0x9)) ) {
        ofs |= 0x6;
    }
    if(F_C || (!F_N && (A > 0x99)) ) {
        ofs |= 0x60;
        carry = 1;
    }
    A += (F_N)? -ofs : ofs;
    F_C = carry;
    F_H = 0;
    F_Z = (A == 0);
    return 4;
}



/// CB Opcodes

int rlc(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x06) {
        u8 v = READ8(HL);
        F_C = (v & 0x80) >> 7;
        v <<= 1;
        v |= F_C;
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 0 && LOWER_NIBBLE <= 7) {
        u8* r8;
        if(LOWER_NIBBLE == 0x0)      r8 = &B;
        else if(LOWER_NIBBLE == 0x1) r8 = &C;
        else if(LOWER_NIBBLE == 0x2) r8 = &D;
        else if(LOWER_NIBBLE == 0x3) r8 = &E;
        else if(LOWER_NIBBLE == 0x4) r8 = &H;
        else if(LOWER_NIBBLE == 0x5) r8 = &L;
        else if(LOWER_NIBBLE == 0x7) r8 = &A;
        else return invalid_instr(op);

        F_C = (*r8 & 0x80) >> 7;
        *r8 <<= 1;
        *r8 |= F_C;
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        return 8;
    }

    return invalid_instr(op);
}

int rl(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x16) {
        u8 v = READ8(HL);
        u8 oc = F_C;
        F_C = (v & 0x80) >> 7;
        v <<= 1;
        v |= oc;
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 1 && LOWER_NIBBLE <= 7) {
        u8* r8;
        if(LOWER_NIBBLE == 0x0)      r8 = &B;
        else if(LOWER_NIBBLE == 0x1) r8 = &C;
        else if(LOWER_NIBBLE == 0x2) r8 = &D;
        else if(LOWER_NIBBLE == 0x3) r8 = &E;
        else if(LOWER_NIBBLE == 0x4) r8 = &H;
        else if(LOWER_NIBBLE == 0x5) r8 = &L;
        else if(LOWER_NIBBLE == 0x7) r8 = &A;
        else return invalid_instr(op);

        u8 oc = F_C;
        F_C = (*r8 & 0x80) >> 7;
        *r8 <<= 1;
        *r8 |= oc;
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        return 8;
    }

    return invalid_instr(op);
}

int rrc(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x0E) {
        u8 v = READ8(HL);
        F_C = v & 0x1;
        v >>= 1;
        v |= (F_C << 7);
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 0 && LOWER_NIBBLE >= 8) {
        u8* r8;
        if(LOWER_NIBBLE == 0x8)      r8 = &B;
        else if(LOWER_NIBBLE == 0x9) r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);

        F_C = *r8 & 0x1;
        *r8 >>= 1;
        *r8 |= (F_C << 7);
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        return 8;
    }

    return invalid_instr(op);
}

int rr(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x1E) {
        u8 v = READ8(HL);
        u8 oc = F_C;
        F_C = v & 0x1;
        v >>= 1;
        v |= (oc << 7);
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 1 && LOWER_NIBBLE >= 8) {
        u8* r8;
        if(LOWER_NIBBLE == 0x8)      r8 = &B;
        else if(LOWER_NIBBLE == 0x9) r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);

        u8 oc = F_C;
        F_C = *r8 & 0x1;
        *r8 >>= 1;
        *r8 |= (oc << 7);
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        return 8;
    }

    return invalid_instr(op);
}

int sla(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x26) {
        u8 v = READ8(HL);
        F_C = (v & 0x80) >> 7;
        v <<= 1;
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 2 && LOWER_NIBBLE <= 7) {
        u8* r8;
        if(LOWER_NIBBLE == 0x0)      r8 = &B;
        else if(LOWER_NIBBLE == 0x1) r8 = &C;
        else if(LOWER_NIBBLE == 0x2) r8 = &D;
        else if(LOWER_NIBBLE == 0x3) r8 = &E;
        else if(LOWER_NIBBLE == 0x4) r8 = &H;
        else if(LOWER_NIBBLE == 0x5) r8 = &L;
        else if(LOWER_NIBBLE == 0x7) r8 = &A;
        else return invalid_instr(op);

        F_C = (*r8 & 0x80) >> 7;
        *r8 <<= 1;
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        return 8;
    }

    return invalid_instr(op);
}

int sra(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x2E) {
        u8 v = READ8(HL);
        F_C = v & 0x1;
        v >>= 1;
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 2 && LOWER_NIBBLE >= 8) {
        u8* r8;
        if(LOWER_NIBBLE == 0x8)      r8 = &B;
        else if(LOWER_NIBBLE == 0x9) r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);

        F_C = *r8 & 0x1;
        *r8 >>= 1;
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        return 8;
    }

    return invalid_instr(op);
}

int srl(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x3E) {
        u8 v = READ8(HL);
        F_C = v & 0x1;
        v >>= 1;
        v |= ((v & 0x40) << 1);
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 3 && LOWER_NIBBLE >= 8) {
        u8* r8;
        if(LOWER_NIBBLE == 0x8)      r8 = &B;
        else if(LOWER_NIBBLE == 0x9) r8 = &C;
        else if(LOWER_NIBBLE == 0xA) r8 = &D;
        else if(LOWER_NIBBLE == 0xB) r8 = &E;
        else if(LOWER_NIBBLE == 0xC) r8 = &H;
        else if(LOWER_NIBBLE == 0xD) r8 = &L;
        else if(LOWER_NIBBLE == 0xF) r8 = &A;
        else return invalid_instr(op);

        F_C = *r8 & 0x1;
        *r8 >>= 1;
        *r8 |= ((*r8 & 0x40) << 1);
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        return 8;
    }

    return invalid_instr(op);
}

int swap(struct cpu* cpu, u8 op) {
    // (HL)
    if(op == 0x36) {
        u8 v = READ8(HL);
        u8 lowbits = v & 0xF;
        v <<= 4;
        v |= lowbits;
        WRITE8(HL, v);
        F_Z = (v == 0);
        F_N = 0;
        F_H = 0;
        F_C = 0;
        return 16;
    }
    // r8
    else if(UPPER_NIBBLE == 3 && LOWER_NIBBLE <= 7) {
        u8* r8;
        if(LOWER_NIBBLE == 0x0)      r8 = &B;
        else if(LOWER_NIBBLE == 0x1) r8 = &C;
        else if(LOWER_NIBBLE == 0x2) r8 = &D;
        else if(LOWER_NIBBLE == 0x3) r8 = &E;
        else if(LOWER_NIBBLE == 0x4) r8 = &H;
        else if(LOWER_NIBBLE == 0x5) r8 = &L;
        else if(LOWER_NIBBLE == 0x7) r8 = &A;
        else return invalid_instr(op);

        u8 lowbits = *r8 & 0xF;
        *r8 <<= 4;
        *r8 |= lowbits;
        F_Z = (*r8 == 0);
        F_N = 0;
        F_H = 0;
        F_C = 0;
        return 8;
    }

    return invalid_instr(op);
}

int bit(struct cpu* cpu, u8 op) {
    u8 bit = 0;
    u8* r8;

    if(UPPER_NIBBLE == 0x4 && LOWER_NIBBLE < 8)       bit = 0;
    else if(UPPER_NIBBLE == 0x4 && LOWER_NIBBLE >= 8) bit = 1;
    else if(UPPER_NIBBLE == 0x5 && LOWER_NIBBLE < 8)  bit = 2;
    else if(UPPER_NIBBLE == 0x5 && LOWER_NIBBLE >= 8) bit = 3;
    else if(UPPER_NIBBLE == 0x6 && LOWER_NIBBLE < 8)  bit = 4;
    else if(UPPER_NIBBLE == 0x6 && LOWER_NIBBLE >= 8) bit = 5;
    else if(UPPER_NIBBLE == 0x7 && LOWER_NIBBLE < 8)  bit = 6;
    else if(UPPER_NIBBLE == 0x7 && LOWER_NIBBLE >= 8) bit = 7;
    else return invalid_instr(op);

    // b, (HL)
    if(LOWER_NIBBLE == 0x6 || LOWER_NIBBLE == 0xE) {
        u8 v = READ8(HL);
        F_Z = (((v >> bit) & 0x1) == 0);
        F_N = 0;
        F_H = 1;
        return 16;
    }
    // b, r8
    else if(LOWER_NIBBLE == 0x0 || LOWER_NIBBLE == 0x8) r8 = &B;
    else if(LOWER_NIBBLE == 0x1 || LOWER_NIBBLE == 0x9) r8 = &C;
    else if(LOWER_NIBBLE == 0x2 || LOWER_NIBBLE == 0xA) r8 = &D;
    else if(LOWER_NIBBLE == 0x3 || LOWER_NIBBLE == 0xB) r8 = &E;
    else if(LOWER_NIBBLE == 0x4 || LOWER_NIBBLE == 0xC) r8 = &H;
    else if(LOWER_NIBBLE == 0x5 || LOWER_NIBBLE == 0xD) r8 = &L;
    else if(LOWER_NIBBLE == 0x7 || LOWER_NIBBLE == 0xF) r8 = &A;
    else return invalid_instr(op);

    F_Z = (((*r8 >> bit) & 0x1) == 0);
    F_N = 0;
    F_H = 1;
    return 8;
}

int res(struct cpu* cpu, u8 op) {
    u8 bit = 0;
    u8* r8;

    if(UPPER_NIBBLE == 0x8 && LOWER_NIBBLE < 8)       bit = 0;
    else if(UPPER_NIBBLE == 0x8 && LOWER_NIBBLE >= 8) bit = 1;
    else if(UPPER_NIBBLE == 0x9 && LOWER_NIBBLE < 8)  bit = 2;
    else if(UPPER_NIBBLE == 0x9 && LOWER_NIBBLE >= 8) bit = 3;
    else if(UPPER_NIBBLE == 0xA && LOWER_NIBBLE < 8)  bit = 4;
    else if(UPPER_NIBBLE == 0xA && LOWER_NIBBLE >= 8) bit = 5;
    else if(UPPER_NIBBLE == 0xB && LOWER_NIBBLE < 8)  bit = 6;
    else if(UPPER_NIBBLE == 0xB && LOWER_NIBBLE >= 8) bit = 7;
    else return invalid_instr(op);

    // b, (HL)
    if(LOWER_NIBBLE == 0x6 || LOWER_NIBBLE == 0xE) {
        u8 v = READ8(HL);
        v &= ~(1 << bit);
        WRITE8(HL, v);
        return 16;
    }
    // b, r8
    else if(LOWER_NIBBLE == 0x0 || LOWER_NIBBLE == 0x8) r8 = &B;
    else if(LOWER_NIBBLE == 0x1 || LOWER_NIBBLE == 0x9) r8 = &C;
    else if(LOWER_NIBBLE == 0x2 || LOWER_NIBBLE == 0xA) r8 = &D;
    else if(LOWER_NIBBLE == 0x3 || LOWER_NIBBLE == 0xB) r8 = &E;
    else if(LOWER_NIBBLE == 0x4 || LOWER_NIBBLE == 0xC) r8 = &H;
    else if(LOWER_NIBBLE == 0x5 || LOWER_NIBBLE == 0xD) r8 = &L;
    else if(LOWER_NIBBLE == 0x7 || LOWER_NIBBLE == 0xF) r8 = &A;
    else return invalid_instr(op);

    *r8 &= ~(1 << bit);
    return 8;
}

int set(struct cpu* cpu, u8 op) {
    u8 bit = 0;
    u8* r8;

    if(UPPER_NIBBLE == 0xC && LOWER_NIBBLE < 8)       bit = 0;
    else if(UPPER_NIBBLE == 0xC && LOWER_NIBBLE >= 8) bit = 1;
    else if(UPPER_NIBBLE == 0xD && LOWER_NIBBLE < 8)  bit = 2;
    else if(UPPER_NIBBLE == 0xD && LOWER_NIBBLE >= 8) bit = 3;
    else if(UPPER_NIBBLE == 0xE && LOWER_NIBBLE < 8)  bit = 4;
    else if(UPPER_NIBBLE == 0xE && LOWER_NIBBLE >= 8) bit = 5;
    else if(UPPER_NIBBLE == 0xF && LOWER_NIBBLE < 8)  bit = 6;
    else if(UPPER_NIBBLE == 0xF && LOWER_NIBBLE >= 8) bit = 7;
    else return invalid_instr(op);

    // b, (HL)
    if(LOWER_NIBBLE == 0x6 || LOWER_NIBBLE == 0xE) {
        u8 v = READ8(HL);
        v |= (1 << bit);
        WRITE8(HL, v);
        return 16;
    }
    // b, r8
    else if(LOWER_NIBBLE == 0x0 || LOWER_NIBBLE == 0x8) r8 = &B;
    else if(LOWER_NIBBLE == 0x1 || LOWER_NIBBLE == 0x9) r8 = &C;
    else if(LOWER_NIBBLE == 0x2 || LOWER_NIBBLE == 0xA) r8 = &D;
    else if(LOWER_NIBBLE == 0x3 || LOWER_NIBBLE == 0xB) r8 = &E;
    else if(LOWER_NIBBLE == 0x4 || LOWER_NIBBLE == 0xC) r8 = &H;
    else if(LOWER_NIBBLE == 0x5 || LOWER_NIBBLE == 0xD) r8 = &L;
    else if(LOWER_NIBBLE == 0x7 || LOWER_NIBBLE == 0xF) r8 = &A;
    else return invalid_instr(op);

    *r8 |= (1 << bit);
    return 8;
}
