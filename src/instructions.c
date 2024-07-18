#include "instructions.h"
#include "cpu.h"
#include "gb.h"
#include "util.h"

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


struct instr_t instructions[512] = {
    // 0x0X
    {1, "NOP"},
    {3, "LD BC, "},
    {1, "LD (BC), A"},
    {1, "INC BC"},
    {1, "INC B"},
    {1, "DEC B"},
    {2, "LD B, "},
    {1, "RLCA"},
    {3, "LD (xxxx), SP  "},
    {1, "ADD HL, BC"},
    {1, "LD A, (BC)"},
    {1, "DEC BC"},
    {1, "INC C"},
    {1, "DEC C"},
    {2, "LD C, "},
    {1, "RRCA"},
    // 0x1X
    {2, "STOP 0"},
    {3, "LD DE, "},
    {1, "LD (DE), A"},
    {1, "INC DE"},
    {1, "INC D"},
    {1, "DEC D"},
    {2, "LD D, "},
    {1, "RLA"},
    {2, "JR "},
    {1, "ADD HL, DE"},
    {1, "LD A, (DE)"},
    {1, "DEC DE"},
    {1, "INC E"},
    {1, "DEC E"},
    {2, "LD E, "},
    {1, "RRA"},
    // 0x2X
    {2, "JR NZ, "},
    {3, "LD HL, "},
    {1, "LDI (HL), A"},
    {1, "INC HL"},
    {1, "INC H"},
    {1, "DEC H"},
    {2, "LD H, "},
    {1, "DAA"},
    {2, "JR Z, "},
    {1, "ADD HL, HL"},
    {1, "LDI A, (HL)"},
    {1, "DEC HL"},
    {1, "INC L"},
    {1, "DEC L"},
    {2, "LD L, "},
    {1, "CPL"},
    // 0x3X
    {2, "JR NC, "},
    {3, "LD SP, "},
    {1, "LDD (HL), A"},
    {1, "INC SP"},
    {1, "INC (HL)"},
    {1, "DEC (HL)"},
    {2, "LD (HL), "},
    {1, "SCF"},
    {2, "JR C, "},
    {1, "ADD HL, SP"},
    {1, "LDD A, (HL)"},
    {1, "DEC SP"},
    {1, "INC A"},
    {1, "DEC A"},
    {2, "LD A, "},
    {1, "CCF"},
    // 0x4X
    {1, "LD B, B"},
    {1, "LD B, C"},
    {1, "LD B, D"},
    {1, "LD B, E"},
    {1, "LD B, H"},
    {1, "LD B, L"},
    {1, "LD B, (HL)"},
    {1, "LD B, A"},
    {1, "LD C, B"},
    {1, "LD C, C"},
    {1, "LD C, D"},
    {1, "LD C, E"},
    {1, "LD C, H"},
    {1, "LD C, L"},
    {1, "LD C, (HL)"},
    {1, "LD C, A"},
    //0x5X
    {1, "LD D, B"},
    {1, "LD D, C"},
    {1, "LD D, D"},
    {1, "LD D, E"},
    {1, "LD D, H"},
    {1, "LD D, L"},
    {1, "LD D, (HL)"},
    {1, "LD D, A"},
    {1, "LD E, B"},
    {1, "LD E, C"},
    {1, "LD E, D"},
    {1, "LD E, E"},
    {1, "LD E, H"},
    {1, "LD E, L"},
    {1, "LD E, (HL)"},
    {1, "LD E, A"},
    // 0x6X
    {1, "LD H, B"},
    {1, "LD H, C"},
    {1, "LD H, D"},
    {1, "LD H, E"},
    {1, "LD H, H"},
    {1, "LD H, L"},
    {1, "LD H, (HL)"},
    {1, "LD H, A"},
    {1, "LD L, B"},
    {1, "LD L, C"},
    {1, "LD L, D"},
    {1, "LD L, E"},
    {1, "LD L, H"},
    {1, "LD L, L"},
    {1, "LD L, (HL)"},
    {1, "LD L, A"},
    // 0x7X
    {1, "LD (HL), B"},
    {1, "LD (HL), C"},
    {1, "LD (HL), D"},
    {1, "LD (HL), E"},
    {1, "LD (HL), H"},
    {1, "LD (HL), L"},
    {1, "HALT"},
    {1, "LD (HL), A"},
    {1, "LD A, B"},
    {1, "LD A, C"},
    {1, "LD A, D"},
    {1, "LD A, E"},
    {1, "LD A, H"},
    {1, "LD A, L"},
    {1, "LD A, (HL)"},
    {1, "LD A, A"},
    // 0x8X
    {1, "ADD A, B"},
    {1, "ADD A, C"},
    {1, "ADD A, D"},
    {1, "ADD A, E"},
    {1, "ADD A, H"},
    {1, "ADD A, L"},
    {1, "ADD A, (HL)"},
    {1, "ADD A, A"},
    {1, "ADC A, B"},
    {1, "ADC A, C"},
    {1, "ADC A, D"},
    {1, "ADC A, E"},
    {1, "ADC A, H"},
    {1, "ADC A, L"},
    {1, "ADC A, (HL)"},
    {1, "ADC A, A"},
    // 0x9X
    {1, "SUB A, B"},
    {1, "SUB A, C"},
    {1, "SUB A, D"},
    {1, "SUB A, E"},
    {1, "SUB A, H"},
    {1, "SUB A, L"},
    {1, "SUB A, (HL)"},
    {1, "SUB A, A"},
    {1, "SBC A, B"},
    {1, "SBC A, C"},
    {1, "SBC A, D"},
    {1, "SBC A, E"},
    {1, "SBC A, H"},
    {1, "SBC A, L"},
    {1, "SBC A, (HL)"},
    {1, "SBC A, A"},
    // 0xAX
    {1, "AND A, B"},
    {1, "AND A, C"},
    {1, "AND A, D"},
    {1, "AND A, E"},
    {1, "AND A, H"},
    {1, "AND A, L"},
    {1, "AND A, (HL)"},
    {1, "AND A, A"},
    {1, "XOR A, B"},
    {1, "XOR A, C"},
    {1, "XOR A, D"},
    {1, "XOR A, E"},
    {1, "XOR A, H"},
    {1, "XOR A, L"},
    {1, "XOR A, (HL)"},
    {1, "XOR A, A"},
    // 0xBX
    {1, "OR A, B"},
    {1, "OR A, C"},
    {1, "OR A, D"},
    {1, "OR A, E"},
    {1, "OR A, H"},
    {1, "OR A, L"},
    {1, "OR A, (HL)"},
    {1, "OR A, A"},
    {1, "CP A, B"},
    {1, "CP A, C"},
    {1, "CP A, D"},
    {1, "CP A, E"},
    {1, "CP A, H"},
    {1, "CP A, L"},
    {1, "CP A, (HL)"},
    {1, "CP A, A"},
    // 0xCX
    {1, "RET NZ"},
    {1, "POP BC"},
    {3, "JP NZ, "},
    {3, "JP "},
    {3, "CALL NZ, "},
    {1, "PUSH BC"},
    {2, "ADD A, "},
    {1, "RST 00H"},
    {1, "RET Z"},
    {1, "RET"},
    {3, "JP Z, "},
    {1, "PREFIX CB"},
    {3, "CALL Z, "},
    {3, "CALL "},
    {2, "ADC A, "},
    {1, "RST 08H"},
    // 0xDX
    {1, "RET NC"},
    {1, "POP DE"},
    {3, "JP NC, "},
    {1, "UNDEFINED"},
    {3, "CALL NC, "},
    {1, "PUSH DE"},
    {2, "SUB A, "},
    {1, "RST 10H"},
    {1, "RET C"},
    {1, "RETI"},
    {3, "JP C, "},
    {1, "UNDEFINED"},
    {3, "CALL C, "},
    {1, "UNDEFINED"},
    {2, "SBC A, "},
    {1, "RST 18H"},
    // 0xEX
    {2, "LDH (FFxx), A  "},
    {1, "POP HL"},
    {1, "LDH (FF00+C), A"},
    {1, "UNDEFINED"},
    {1, "UNDEFINED"},
    {1, "PUSH HL"},
    {2, "AND A, "},
    {1, "RST 20H"},
    {2, "ADD SP, "},
    {1, "JP HL"},
    {3, "LD (xxxx), A  "},
    {1, "UNDEFINED"},
    {1, "UNDEFINED"},
    {1, "UNDEFINED"},
    {2, "XOR A, "},
    {1, "RST 28H"},
    // 0xFX
    {2, "LDH A, (FFxx)  "},
    {1, "POP AF"},
    {1, "LDH A, (FF00+C)"},
    {1, "DI"},
    {1, "UNDEFINED"},
    {1, "PUSH AF"},
    {2, "OR A, "},
    {1, "RST 30H"},
    {2, "LD HL, SP+"},
    {1, "LD SP, HL"},
    {3, "LD A, (xxxx)  "},
    {1, "EI"},
    {1, "UNDEFINED"},
    {1, "UNDEFINED"},
    {2, "CP A, "},
    {1, "RST 38H"},

    // CB Opcodes
    // 0x0X
    {1, "RLC B"},
    {1, "RLC C"},
    {1, "RLC D"},
    {1, "RLC E"},
    {1, "RLC H"},
    {1, "RLC L"},
    {1, "RLC (HL)"},
    {1, "RLC A"},
    {1, "RRC B"},
    {1, "RRC C"},
    {1, "RRC D"},
    {1, "RRC E"},
    {1, "RRC H"},
    {1, "RRC L"},
    {1, "RRC (HL)"},
    {1, "RRC A"},
    // 0x1X
    {1, "RL B"},
    {1, "RL C"},
    {1, "RL D"},
    {1, "RL E"},
    {1, "RL H"},
    {1, "RL L"},
    {1, "RL (HL)"},
    {1, "RL A"},
    {1, "RR B"},
    {1, "RR C"},
    {1, "RR D"},
    {1, "RR E"},
    {1, "RR H"},
    {1, "RR L"},
    {1, "RR (HL)"},
    {1, "RR A"},
    // 0x2X
    {1, "SLA B"},
    {1, "SLA C"},
    {1, "SLA D"},
    {1, "SLA E"},
    {1, "SLA H"},
    {1, "SLA L"},
    {1, "SLA (HL)"},
    {1, "SLA A"},
    {1, "SRA B"},
    {1, "SRA C"},
    {1, "SRA D"},
    {1, "SRA E"},
    {1, "SRA H"},
    {1, "SRA L"},
    {1, "SRA (HL)"},
    {1, "SRA A"},
    // 0x3X
    {1, "SWAP B"},
    {1, "SWAP C"},
    {1, "SWAP D"},
    {1, "SWAP E"},
    {1, "SWAP H"},
    {1, "SWAP L"},
    {1, "SWAP (HL)"},
    {1, "SWAP A"},
    {1, "SRL B"},
    {1, "SRL C"},
    {1, "SRL D"},
    {1, "SRL E"},
    {1, "SRL H"},
    {1, "SRL L"},
    {1, "SRL (HL)"},
    {1, "SRL A"},
    // 0x4X
    {1, "BIT 0, B"},
    {1, "BIT 0, C"},
    {1, "BIT 0, D"},
    {1, "BIT 0, E"},
    {1, "BIT 0, H"},
    {1, "BIT 0, L"},
    {1, "BIT 0, (HL)"},
    {1, "BIT 0, A"},
    {1, "BIT 1, B"},
    {1, "BIT 1, C"},
    {1, "BIT 1, D"},
    {1, "BIT 1, E"},
    {1, "BIT 1, H"},
    {1, "BIT 1, L"},
    {1, "BIT 1, (HL)"},
    {1, "BIT 1, A"},
    // 0x5X
    {1, "BIT 2, B"},
    {1, "BIT 2, C"},
    {1, "BIT 2, D"},
    {1, "BIT 2, E"},
    {1, "BIT 2, H"},
    {1, "BIT 2, L"},
    {1, "BIT 2, (HL)"},
    {1, "BIT 2, A"},
    {1, "BIT 3, B"},
    {1, "BIT 3, C"},
    {1, "BIT 3, D"},
    {1, "BIT 3, E"},
    {1, "BIT 3, H"},
    {1, "BIT 3, L"},
    {1, "BIT 3, (HL)"},
    {1, "BIT 3, A"},
    // 0x6X
    {1, "BIT 4, B"},
    {1, "BIT 4, C"},
    {1, "BIT 4, D"},
    {1, "BIT 4, E"},
    {1, "BIT 4, H"},
    {1, "BIT 4, L"},
    {1, "BIT 4, (HL)"},
    {1, "BIT 4, A"},
    {1, "BIT 5, B"},
    {1, "BIT 5, C"},
    {1, "BIT 5, D"},
    {1, "BIT 5, E"},
    {1, "BIT 5, H"},
    {1, "BIT 5, L"},
    {1, "BIT 5, (HL)"},
    {1, "BIT 5, A"},
    // 0x7X
    {1, "BIT 6, B"},
    {1, "BIT 6, C"},
    {1, "BIT 6, D"},
    {1, "BIT 6, E"},
    {1, "BIT 6, H"},
    {1, "BIT 6, L"},
    {1, "BIT 6, (HL)"},
    {1, "BIT 6, A"},
    {1, "BIT 7, B"},
    {1, "BIT 7, C"},
    {1, "BIT 7, D"},
    {1, "BIT 7, E"},
    {1, "BIT 7, H"},
    {1, "BIT 7, L"},
    {1, "BIT 7, (HL)"},
    {1, "BIT 7, A"},
    // 0x8X
    {1, "RES 0, B"},
    {1, "RES 0, C"},
    {1, "RES 0, D"},
    {1, "RES 0, E"},
    {1, "RES 0, H"},
    {1, "RES 0, L"},
    {1, "RES 0, (HL)"},
    {1, "RES 0, A"},
    {1, "RES 1, B"},
    {1, "RES 1, C"},
    {1, "RES 1, D"},
    {1, "RES 1, E"},
    {1, "RES 1, H"},
    {1, "RES 1, L"},
    {1, "RES 1, (HL)"},
    {1, "RES 1, A"},
    // 0x9X
    {1, "RES 2, B"},
    {1, "RES 2, C"},
    {1, "RES 2, D"},
    {1, "RES 2, E"},
    {1, "RES 2, H"},
    {1, "RES 2, L"},
    {1, "RES 2, (HL)"},
    {1, "RES 2, A"},
    {1, "RES 3, B"},
    {1, "RES 3, C"},
    {1, "RES 3, D"},
    {1, "RES 3, E"},
    {1, "RES 3, H"},
    {1, "RES 3, L"},
    {1, "RES 3, (HL)"},
    {1, "RES 3, A"},
    // 0xAX
    {1, "RES 4, B"},
    {1, "RES 4, C"},
    {1, "RES 4, D"},
    {1, "RES 4, E"},
    {1, "RES 4, H"},
    {1, "RES 4, L"},
    {1, "RES 4, (HL)"},
    {1, "RES 4, A"},
    {1, "RES 5, B"},
    {1, "RES 5, C"},
    {1, "RES 5, D"},
    {1, "RES 5, E"},
    {1, "RES 5, H"},
    {1, "RES 5, L"},
    {1, "RES 5, (HL)"},
    {1, "RES 5, A"},
    // 0xBX
    {1, "RES 6, B"},
    {1, "RES 6, C"},
    {1, "RES 6, D"},
    {1, "RES 6, E"},
    {1, "RES 6, H"},
    {1, "RES 6, L"},
    {1, "RES 6, (HL)"},
    {1, "RES 6, A"},
    {1, "RES 7, B"},
    {1, "RES 7, C"},
    {1, "RES 7, D"},
    {1, "RES 7, E"},
    {1, "RES 7, H"},
    {1, "RES 7, L"},
    {1, "RES 7, (HL)"},
    {1, "RES 7, A"},
    // 0xCX
    {1, "SET 0, B"},
    {1, "SET 0, C"},
    {1, "SET 0, D"},
    {1, "SET 0, E"},
    {1, "SET 0, H"},
    {1, "SET 0, L"},
    {1, "SET 0, (HL)"},
    {1, "SET 0, A"},
    {1, "SET 1, B"},
    {1, "SET 1, C"},
    {1, "SET 1, D"},
    {1, "SET 1, E"},
    {1, "SET 1, H"},
    {1, "SET 1, L"},
    {1, "SET 1, (HL)"},
    {1, "SET 1, A"},
    // 0xDX
    {1, "SET 2, B"},
    {1, "SET 2, C"},
    {1, "SET 2, D"},
    {1, "SET 2, E"},
    {1, "SET 2, H"},
    {1, "SET 2, L"},
    {1, "SET 2, (HL)"},
    {1, "SET 2, A"},
    {1, "SET 3, B"},
    {1, "SET 3, C"},
    {1, "SET 3, D"},
    {1, "SET 3, E"},
    {1, "SET 3, H"},
    {1, "SET 3, L"},
    {1, "SET 3, (HL)"},
    {1, "SET 3, A"},
    // 0xEX
    {1, "SET 4, B"},
    {1, "SET 4, C"},
    {1, "SET 4, D"},
    {1, "SET 4, E"},
    {1, "SET 4, H"},
    {1, "SET 4, L"},
    {1, "SET 4, (HL)"},
    {1, "SET 4, A"},
    {1, "SET 5, B"},
    {1, "SET 5, C"},
    {1, "SET 5, D"},
    {1, "SET 5, E"},
    {1, "SET 5, H"},
    {1, "SET 5, L"},
    {1, "SET 5, (HL)"},
    {1, "SET 5, A"},
    // 0xFX
    {1, "SET 6, B"},
    {1, "SET 6, C"},
    {1, "SET 6, D"},
    {1, "SET 6, E"},
    {1, "SET 6, H"},
    {1, "SET 6, L"},
    {1, "SET 6, (HL)"},
    {1, "SET 6, A"},
    {1, "SET 7, B"},
    {1, "SET 7, C"},
    {1, "SET 7, D"},
    {1, "SET 7, E"},
    {1, "SET 7, H"},
    {1, "SET 7, L"},
    {1, "SET 7, (HL)"},
    {1, "SET 7, A"}
};

#define INV() \
    printf("Warning: attempting to execute invalid opcode %02X", opcode); \
    return -1;
// Misc
#define NOP() \
    return 4;
#define STOP() \
    /*TODO: Implement */ \
    return 4;
#define HALT() \
    /*TODO: Implement */ \
    return 4;
#define EI() \
    cpu->imeWait = 1; \
    return 4;
#define DI() \
    cpu->ime = false; \
    return 4;
#define CB() \
    u8 nextop = READ8(PC++); \
    return execute_CB(cpu, nextop);

// Incs
#define INC(reg) \
    F_H = (((reg & 0xF) + 1) & 0x10) == 0x10; \
    reg++; \
    F_Z = (reg == 0); \
    F_N = 0; \
    return 4;
#define INC_R16(reg) \
    reg++; \
    return 8;
#define INC_XHL() \
    u8 d = READ8(HL); \
    F_H = (((d & 0xF) + 1) & 0x10) == 0x10; \
    d++; \
    F_Z = (d == 0); \
    F_N = 0; \
    WRITE8(HL, d); \
    return 12;

// Decs
#define DEC(reg) \
    F_H = ((reg & 0xF) < 1); \
    reg--; \
    F_Z = (reg == 0); \
    F_N = 1; \
    return 4;
#define DEC_R16(reg) \
    reg--; \
    return 8;
#define DEC_XHL() \
    u8 d = READ8(HL); \
    F_H = ((d & 0xF) < 1); \
    d--; \
    F_Z = (d == 0); \
    F_N = 1; \
    WRITE8(HL, d); \
    return 12;

// Adds
#define ADD_R16_R16(r1, r2) \
    F_C = ((r1 + r2) & 0x10000) == 0x10000; \
    F_H = (((r1 & 0xFFF) + (r2 & 0xFFF)) & 0x1000) == 0x1000; \
    r1 += r2; \
    F_N = 0; \
    return 8;
#define ADD_SP_S8() \
    s8 s = READ8(PC); \
    PC++; \
    F_C = ((SP + s) & 0x10000) == 0x10000; \
    F_H = (((SP & 0xFFF) + s) & 0x1000) == 0x1000; \
    SP += s; \
    F_Z = 0; \
    F_N = 0; \
    return 16;
#define ADD(reg) \
    F_C = ((A + reg) & 0x100) == 0x100; \
    F_H = (((A & 0xF) + (reg & 0xF)) & 0x10) == 0x10; \
    A += reg; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 4;
#define ADD_HL() \
    u8 byte = READ8(HL); \
    F_C = ((A + byte) & 0x100) == 0x100; \
    F_H = (((A & 0xF) + (byte & 0xF)) & 0x10) == 0x10; \
    A += byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;
#define ADD_U8() \
    u8 byte = READ8(PC); \
    PC++; \
    F_C = ((A + byte) & 0x100) == 0x100; \
    F_H = (((A & 0xF) + (byte & 0xF)) & 0x10) == 0x10; \
    A += byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;

#define ADC(reg) \
    u8 oc = F_C; \
    F_C = ((A + reg + oc) & 0x100) == 0x100; \
    F_H = (((A & 0xF) + (reg & 0xF) + oc) & 0x10) == 0x10; \
    A += reg + oc; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 4;
#define ADC_HL() \
    u8 oc = F_C; \
    u8 byte = READ8(HL); \
    F_C = ((A + byte + oc) & 0x100) == 0x100; \
    F_H = (((A & 0xF) + (byte & 0xF) + oc) & 0x10) == 0x10; \
    A += byte + oc; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;
#define ADC_U8() \
    u8 oc = F_C; \
    u8 byte = READ8(PC); \
    PC++; \
    F_C = ((A + byte + oc) & 0x100) == 0x100; \
    F_H = (((A & 0xF) + (byte & 0xF) + oc) & 0x10) == 0x10; \
    A += byte + oc; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;

// Subs
#define SUB(reg) \
    F_C = (A < reg); \
    F_H = ((A & 0xF) < (reg & 0xF)); \
    A -= reg; \
    F_Z = (A == 0); \
    F_N = 1; \
    return 4;
#define SUB_HL() \
    u8 byte = READ8(HL); \
    F_C = (A < byte); \
    F_H = ((A & 0xF) < (byte & 0xF)); \
    A -= byte; \
    F_Z = (A == 0); \
    F_N = 1; \
    return 8;
#define SUB_U8() \
    u8 byte = READ8(PC); \
    PC++; \
    F_C = (A < byte); \
    F_H = ((A & 0xF) < (byte & 0xF)); \
    A -= byte; \
    F_Z = (A == 0); \
    F_N = 1; \
    return 8;

#define SBC(reg) \
    u8 oc = F_C; \
    F_C = (A < (reg + oc)); \
    F_H = ((A & 0xF) < ((reg & 0xF) + oc)); \
    A -= (reg + oc); \
    F_Z = (A == 0); \
    F_N = 1; \
    return 4;
#define SBC_HL() \
    u8 byte = READ8(HL); \
    u8 oc = F_C; \
    F_C = (A < (byte + oc)); \
    F_H = ((A & 0xF) < ((byte & 0xF) + oc)); \
    A -= (byte + oc); \
    F_Z = (A == 0); \
    F_N = 1; \
    return 8;
#define SBC_U8() \
    u8 byte = READ8(PC); \
    PC++; \
    u8 oc = F_C; \
    F_C = (A < (byte + oc)); \
    F_H = ((A & 0xF) < ((byte & 0xF) + oc)); \
    A -= (byte + oc); \
    F_Z = (A == 0); \
    F_N = 1; \
    return 8;

// Loads
#define LD_R16_U16(reg) \
    reg = READ16(PC); \
    PC += 2; \
    return 12;
#define LD_R16_R16(reg1, reg2) \
    reg1 = reg2; \
    return 8;

#define LD_R_U8(reg) \
    reg = READ8(PC); \
    PC++; \
    return 8;
#define LD_R_R(reg1, reg2) \
    reg1 = reg2; \
    return 4;

#define LD_XR16_R(r16, reg) \
    WRITE8(r16, reg); \
    return 8;
#define LD_R_XR16(reg, r16) \
    reg = READ8(r16); \
    return 8;

#define LD_XU16_A() \
    WRITE8(READ16(PC), A); \
    PC += 2; \
    return 16;
#define LD_A_XU16() \
    A = READ8(READ16(PC)); \
    PC += 2; \
    return 16;

#define LD_XC_A() \
    WRITE8(0xFF00 + C, A); \
    return 8;
#define LD_A_XC() \
    A = READ8(0xFF00 + C); \
    return 8;
#define LD_XU8_A() \
    WRITE8(0xFF00 + READ8(PC), A); \
    PC++; \
    return 12;
#define LD_A_XU8() \
    A = READ8(0xFF00 + READ8(PC)); \
    PC++; \
    return 12;

#define LD_XR16_U8(r16) \
    WRITE8(r16, READ8(PC)); \
    PC++; \
    return 12;
#define LD_XU16_SP() \
    WRITE16(READ16(PC), SP); \
    PC += 2; \
    return 20;
#define LD_HL_SP_S8() \
    s8 s = READ8(PC); \
    PC++; \
    F_C = ((SP + s) & 0x10000) == 0x10000; \
    F_H = (((SP & 0xFFF) + s) & 0x1000) == 0x1000; \
    HL = SP + s; \
    F_Z = 0; \
    F_N = 0; \
    return 12;

#define LDI_XHL_A() \
    WRITE8(HL, A); \
    HL++; \
    return 8;
#define LDD_XHL_A() \
    WRITE8(HL, A); \
    HL--; \
    return 8;
#define LDI_A_XHL() \
    A = READ8(HL); \
    HL++; \
    return 8;
#define LDD_A_XHL() \
    A = READ8(HL); \
    HL--; \
    return 8;

// Jumps
#define JP_HL() \
    PC = HL; \
    return 4;
#define JP_U16() \
    u16 addr = READ16(PC); \
    PC = addr; \
    return 16;
#define JPZ_U16() \
    if(F_Z) { \
        u16 addr = READ16(PC); \
        PC = addr; \
        return 16; \
    } else { \
        PC += 2; \
        return 12; \
    }
#define JPC_U16() \
    if(F_C) { \
        u16 addr = READ16(PC); \
        PC = addr; \
        return 16; \
    } else { \
        PC += 2; \
        return 12; \
    }
#define JPNZ_U16() \
    if(!F_Z) { \
        u16 addr = READ16(PC); \
        PC = addr; \
        return 16; \
    } else { \
        PC += 2; \
        return 12; \
    }
#define JPNC_U16() \
    if(!F_C) { \
        u16 addr = READ16(PC); \
        PC = addr; \
        return 16; \
    } else { \
        PC += 2; \
        return 12; \
    }

// Relative jumps
#define JR_S8() \
    s8 ofs = READ8(PC); \
    PC++; \
    PC += ofs; \
    return 12;
#define JRZ_S8() \
    s8 ofs = READ8(PC); \
    PC++; \
    if(F_Z) { \
        PC += ofs; \
        return 12; \
    } else { \
        return 8; \
    }
#define JRC_S8() \
    s8 ofs = READ8(PC); \
    PC++; \
    if(F_C) { \
        PC += ofs; \
        return 12; \
    } else { \
        return 8; \
    }
#define JRNZ_S8() \
    s8 ofs = READ8(PC); \
    PC++; \
    if(!F_Z) { \
        PC += ofs; \
        return 12; \
    } else { \
        return 8; \
    }
#define JRNC_S8() \
    s8 ofs = READ8(PC); \
    PC++; \
    if(!F_C) { \
        PC += ofs; \
        return 12; \
    } else { \
        return 8; \
    }

// Calls
#define CALL() \
    u16 nextInstrAddr = PC + 2; \
    WRITE8(SP - 1, (nextInstrAddr & 0xFF00) >> 8); \
    WRITE8(SP - 2, nextInstrAddr & 0xFF); \
    u16 addr = READ16(PC); \
    PC = addr; \
    SP -= 2; \
    return 24;
#define CALLZ() \
    if(F_Z) { \
        u16 nextInstrAddr = PC + 2; \
        WRITE8(SP - 1, (nextInstrAddr & 0xFF00) >> 8); \
        WRITE8(SP - 2, nextInstrAddr & 0xFF); \
        u16 addr = READ16(PC); \
        PC = addr; \
        SP -= 2; \
        return 24; \
    } else { \
        PC += 2; \
        return 12; \
    }
#define CALLC() \
    if(F_C) { \
        u16 nextInstrAddr = PC + 2; \
        WRITE8(SP - 1, (nextInstrAddr & 0xFF00) >> 8); \
        WRITE8(SP - 2, nextInstrAddr & 0xFF); \
        u16 addr = READ16(PC); \
        PC = addr; \
        SP -= 2; \
        return 24; \
    } else { \
        PC += 2; \
        return 12; \
    }
#define CALLNZ() \
    if(!F_Z) { \
        u16 nextInstrAddr = PC + 2; \
        WRITE8(SP - 1, (nextInstrAddr & 0xFF00) >> 8); \
        WRITE8(SP - 2, nextInstrAddr & 0xFF); \
        u16 addr = READ16(PC); \
        PC = addr; \
        SP -= 2; \
        return 24; \
    } else { \
        PC += 2; \
        return 12; \
    }
#define CALLNC() \
    if(!F_C) { \
        u16 nextInstrAddr = PC + 2; \
        WRITE8(SP - 1, (nextInstrAddr & 0xFF00) >> 8); \
        WRITE8(SP - 2, nextInstrAddr & 0xFF); \
        u16 addr = READ16(PC); \
        PC = addr; \
        SP -= 2; \
        return 24; \
    } else { \
        PC += 2; \
        return 12; \
    }

// Returns
#define RET() \
    PC = (READ8(SP) | (READ8(SP + 1) << 8)); \
    SP += 2; \
    return 16;
#define RETI() \
    PC = (READ8(SP) | (READ8(SP + 1) << 8)); \
    SP += 2; \
    cpu->imeWait = 1; /* TODO: Should RETI wait or set ime immediately? */ \
    return 16;
#define RETZ() \
    if(F_Z) { \
        PC = (READ8(SP) | (READ8(SP + 1) << 8)); \
        SP += 2; \
        return 20; \
    } else { \
        return 8; \
    }
#define RETC() \
    if(F_C) { \
        PC = (READ8(SP) | (READ8(SP + 1) << 8)); \
        SP += 2; \
        return 20; \
    } else { \
        return 8; \
    }
#define RETNZ() \
    if(!F_Z) { \
        PC = (READ8(SP) | (READ8(SP + 1) << 8)); \
        SP += 2; \
        return 20; \
    } else { \
        return 8; \
    }
#define RETNC() \
    if(!F_C) { \
        PC = (READ8(SP) | (READ8(SP + 1) << 8)); \
        SP += 2; \
        return 20; \
    } else { \
        return 8; \
    }

// Push/pop
#define PUSH(reg) \
    WRITE8(SP - 1, reg >> 8); \
    WRITE8(SP - 2, reg & 0xFF); \
    SP -= 2; \
    return 16;
#define POP(reg) \
    reg = (READ8(SP) | (READ8(SP + 1) << 8)); \
    SP += 2; \
    return 12;
#define POP_AF() \
    AF = (READ8(SP) | (READ8(SP + 1) << 8)); \
    /* Lower portion of F must be 0 */ \
    F_X = 0; \
    SP += 2; \
    return 12;

// RST
#define RST() \
    /* Encoding is 00xxx000, where xxx is multiplied by 8 for the zero-page offset we jump to */ \
    u8 offset = (((opcode & 0x38) >> 3) & 0x7) * 8; \
    WRITE8(SP - 1, (PC & 0xFF00) >> 8); \
    WRITE8(SP - 2, PC & 0xFF); \
    SP -= 2; \
    PC = offset; \
    return 16;

// Rotates on acc
#define RLCA() \
    F_C = (A & 0x80) >> 7; \
    A <<= 1; \
    A |= F_C; \
    F_Z = 0; \
    F_N = 0; \
    F_H = 0; \
    return 4;
#define RLA() \
    u8 oc = F_C; \
    F_C = (A & 0x80) >> 7; \
    A <<= 1; \
    A |= oc; \
    F_Z = 0; \
    F_N = 0; \
    F_H = 0; \
    return 4;
#define RRCA() \
    F_C = A & 1; \
    A >>= 1; \
    A |= (F_C << 7); \
    F_Z = 0; \
    F_N = 0; \
    F_H = 0; \
    return 4;
#define RRA() \
    u8 oc = F_C; \
    F_C = A & 1; \
    A >>= 1; \
    A |= (oc << 7); \
    F_Z = 0; \
    F_N = 0; \
    F_H = 0; \
    return 4;

// Carry modifiers
#define SCF() \
    F_C = 1; \
    F_N = 0; \
    F_H = 0; \
    return 4;
#define CCF() \
    F_C = ~F_C; \
    F_N = 0; \
    F_H = 0; \
    return 4;

// ANDs
#define AND(reg) \
    F_C = 0; \
    F_H = 1; \
    A &= reg; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 4;
#define AND_XHL() \
    u8 byte = READ8(HL); \
    F_C = 0; \
    F_H = 1; \
    A &= byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;
#define AND_U8() \
    u8 byte = READ8(PC); \
    PC++; \
    F_C = 0; \
    F_H = 1; \
    A &= byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;

// XORs
#define XOR(reg) \
    F_C = 0; \
    F_H = 0; \
    A ^= reg; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 4;
#define XOR_XHL() \
    u8 byte = READ8(HL); \
    F_C = 0; \
    F_H = 0; \
    A ^= byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;
#define XOR_U8() \
    u8 byte = READ8(PC); \
    PC++; \
    F_C = 0; \
    F_H = 0; \
    A ^= byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;

// ORs
#define OR(reg) \
    F_C = 0; \
    F_H = 0; \
    A |= reg; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 4;
#define OR_XHL() \
    u8 byte = READ8(HL); \
    F_C = 0; \
    F_H = 0; \
    A |= byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;
#define OR_U8() \
    u8 byte = READ8(PC); \
    PC++; \
    F_C = 0; \
    F_H = 0; \
    A |= byte; \
    F_Z = (A == 0); \
    F_N = 0; \
    return 8;

// Compares
#define CP(reg) \
    F_C = (A < reg); \
    F_H = ((A & 0xF) < (reg & 0xF)); \
    F_Z = (A - reg) == 0; \
    F_N = 1; \
    return 4;
#define CP_XHL() \
    u8 byte = READ8(HL); \
    F_C = (A < byte); \
    F_H = ((A & 0xF) < (byte & 0xF)); \
    F_Z = (A - byte) == 0; \
    F_N = 1; \
    return 8;
#define CP_U8() \
    u8 byte = READ8(PC); \
    PC++; \
    F_C = (A < byte); \
    F_H = ((A & 0xF) < (byte & 0xF)); \
    F_Z = (A - byte) == 0; \
    F_N = 1; \
    return 8;

// One's complement
#define CPL() \
    A = ~A; \
    F_N = 1; \
    F_H = 1; \
    return 4;

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
#define DAA() \
    u8 ofs = 0; \
    u8 carry = 0; \
    if(F_H || (!F_N && ((A & 0xF) > 0x9)) ) { \
        ofs |= 0x6; \
    } \
    if(F_C || (!F_N && (A > 0x99)) ) { \
        ofs |= 0x60; \
        carry = 1; \
    } \
    A += (F_N)? -ofs : ofs; \
    F_C = carry; \
    F_H = 0; \
    F_Z = (A == 0); \
    return 4;


//////////////
///// CB /////
//////////////

// Rotates
#define RLC(reg) \
    F_C = (reg & 0x80) >> 7; \
    reg <<= 1; \
    reg |= F_C; \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    return 8;
#define RLC_HL() \
    u8 byte = READ8(HL); \
    F_C = (byte & 0x80) >> 7; \
    byte <<= 1; \
    byte |= F_C; \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    return 16;
#define RL(reg) \
    u8 oc = F_C; \
    F_C = (reg & 0x80) >> 7; \
    reg <<= 1; \
    reg |= oc; \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    return 8;
#define RL_HL() \
    u8 byte = READ8(HL); \
    u8 oc = F_C; \
    F_C = (byte & 0x80) >> 7; \
    byte <<= 1; \
    byte |= oc; \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    return 16;
#define RRC(reg) \
    F_C = reg & 0x1; \
    reg >>= 1; \
    reg |= (F_C << 7); \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    return 8;
#define RRC_HL() \
    u8 byte = READ8(HL); \
    F_C = byte & 0x1; \
    byte >>= 1; \
    byte |= (F_C << 7); \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    return 16;
#define RR(reg) \
    u8 oc = F_C; \
    F_C = reg & 0x1; \
    reg >>= 1; \
    reg |= (oc << 7); \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    return 8;
#define RR_HL() \
    u8 byte = READ8(HL); \
    u8 oc = F_C; \
    F_C = byte & 0x1; \
    byte >>= 1; \
    byte |= (oc << 7); \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    return 16;

// Shifts
#define SLA(reg) \
    F_C = (reg & 0x80) >> 7; \
    reg <<= 1; \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    return 8;
#define SLA_HL() \
    u8 byte = READ8(HL); \
    F_C = (byte & 0x80) >> 7; \
    byte <<= 1; \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    return 16;
#define SRA(reg) \
    F_C = reg & 0x1; \
    reg >>= 1; \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    return 8;
#define SRA_HL() \
    u8 byte = READ8(HL); \
    F_C = byte & 0x1; \
    byte >>= 1; \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    return 16;
#define SRL(reg) \
    F_C = reg & 0x1; \
    reg >>= 1; \
    reg |= ((reg & 0x40) << 1); \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    return 8;
#define SRL_HL() \
    u8 byte = READ8(HL); \
    F_C = byte & 0x1; \
    byte >>= 1; \
    byte |= ((byte & 0x40) << 1); \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    return 16;

// SWAP
#define SWAP(reg) \
    u8 lowbits = reg & 0xF; \
    reg >>= 4; \
    reg |= (lowbits << 4); \
    F_Z = (reg == 0); \
    F_N = 0; \
    F_H = 0; \
    F_C = 0; \
    return 8;
#define SWAP_HL() \
    u8 byte = READ8(HL); \
    u8 lowbits = byte & 0xF; \
    byte >>= 4; \
    byte |= (lowbits << 4); \
    WRITE8(HL, byte); \
    F_Z = (byte == 0); \
    F_N = 0; \
    F_H = 0; \
    F_C = 0; \
    return 16;

// Bit tests
#define BIT(bit, reg) \
    F_Z = (((reg >> bit) & 0x1) == 0); \
    F_N = 0; \
    F_H = 1; \
    return 8;
#define BIT_HL(bit) \
    u8 byte = READ8(HL); \
    F_Z = (((byte >> bit) & 0x1) == 0); \
    F_N = 0; \
    F_H = 1; \
    return 16;

// Bit resets
#define RES(bit, reg) \
    reg &= ~(1 << bit); \
    return 8;
#define RES_HL(bit) \
    u8 byte = READ8(HL); \
    byte &= ~(1 << bit); \
    WRITE8(HL, byte); \
    return 16;

// Bit sets
#define SET(bit, reg) \
    reg |= (1 << bit); \
    return 8;
#define SET_HL(bit) \
    u8 byte = READ8(HL); \
    byte |= (1 << bit); \
    WRITE8(HL, byte); \
    return 16;




int execute_instr(struct cpu* cpu) {

    u8 opcode = READ8(cpu->pc);

    if(cpu->loggingEnabled) {
        log_instruction_line(cpu, PC, opcode);
        if(opcode == 0xCB) {
            log_instruction_line(cpu, PC + 1, READ8(cpu->pc + 1) + 256);
        }
    }

    cpu->pc++;

    switch(opcode) {
        case 0x00: { NOP();            } case 0x01: { LD_R16_U16(BC);      } case 0x02: { LD_XR16_R(BC, A); } case 0x03: { INC_R16(BC);      } case 0x04: { INC(B);           } case 0x05: { DEC(B);           } case 0x06: { LD_R_U8(B);       } case 0x07: { RLCA();           }
        case 0x08: { LD_XU16_SP();     } case 0x09: { ADD_R16_R16(HL, BC); } case 0x0A: { LD_R_XR16(A, BC); } case 0x0B: { DEC_R16(BC);      } case 0x0C: { INC(C);           } case 0x0D: { DEC(C);           } case 0x0E: { LD_R_U8(C);       } case 0x0F: { RRCA();           }
        case 0x10: { STOP();           } case 0x11: { LD_R16_U16(DE);      } case 0x12: { LD_XR16_R(DE, A); } case 0x13: { INC_R16(DE);      } case 0x14: { INC(D);           } case 0x15: { DEC(D);           } case 0x16: { LD_R_U8(D);       } case 0x17: { RLA();            }
        case 0x18: { JR_S8();          } case 0x19: { ADD_R16_R16(HL, DE); } case 0x1A: { LD_R_XR16(A, DE); } case 0x1B: { DEC_R16(DE);      } case 0x1C: { INC(E);           } case 0x1D: { DEC(E);           } case 0x1E: { LD_R_U8(E);       } case 0x1F: { RRA();            }
        case 0x20: { JRNZ_S8();        } case 0x21: { LD_R16_U16(HL);      } case 0x22: { LDI_XHL_A();      } case 0x23: { INC_R16(HL);      } case 0x24: { INC(H);           } case 0x25: { DEC(H);           } case 0x26: { LD_R_U8(H);       } case 0x27: { DAA();            }
        case 0x28: { JRZ_S8();         } case 0x29: { ADD_R16_R16(HL, HL); } case 0x2A: { LDI_A_XHL();      } case 0x2B: { DEC_R16(HL);      } case 0x2C: { INC(L);           } case 0x2D: { DEC(L);           } case 0x2E: { LD_R_U8(L);       } case 0x2F: { CPL();            }
        case 0x30: { JRNC_S8();        } case 0x31: { LD_R16_U16(SP);      } case 0x32: { LDD_XHL_A();      } case 0x33: { INC_R16(SP);      } case 0x34: { INC_XHL();        } case 0x35: { DEC_XHL();        } case 0x36: { LD_XR16_U8(HL);   } case 0x37: { SCF();            }
        case 0x38: { JRC_S8();         } case 0x39: { ADD_R16_R16(HL, SP); } case 0x3A: { LDD_A_XHL();      } case 0x3B: { DEC_R16(SP);      } case 0x3C: { INC(A);           } case 0x3D: { DEC(A);           } case 0x3E: { LD_R_U8(A);       } case 0x3F: { CCF();            }
        case 0x40: { LD_R_R(B, B);     } case 0x41: { LD_R_R(B, C);        } case 0x42: { LD_R_R(B, D);     } case 0x43: { LD_R_R(B, E);     } case 0x44: { LD_R_R(B, H);     } case 0x45: { LD_R_R(B, L);     } case 0x46: { LD_R_XR16(B, HL); } case 0x47: { LD_R_R(B, A);     }
        case 0x48: { LD_R_R(C, B);     } case 0x49: { LD_R_R(C, C);        } case 0x4A: { LD_R_R(C, D);     } case 0x4B: { LD_R_R(C, E);     } case 0x4C: { LD_R_R(C, H);     } case 0x4D: { LD_R_R(C, L);     } case 0x4E: { LD_R_XR16(C, HL); } case 0x4F: { LD_R_R(C, A);     }
        case 0x50: { LD_R_R(D, B);     } case 0x51: { LD_R_R(D, C);        } case 0x52: { LD_R_R(D, D);     } case 0x53: { LD_R_R(D, E);     } case 0x54: { LD_R_R(D, H);     } case 0x55: { LD_R_R(D, L);     } case 0x56: { LD_R_XR16(D, HL); } case 0x57: { LD_R_R(D, A);     }
        case 0x58: { LD_R_R(E, B);     } case 0x59: { LD_R_R(E, C);        } case 0x5A: { LD_R_R(E, D);     } case 0x5B: { LD_R_R(E, E);     } case 0x5C: { LD_R_R(E, H);     } case 0x5D: { LD_R_R(E, L);     } case 0x5E: { LD_R_XR16(E, HL); } case 0x5F: { LD_R_R(E, A);     }
        case 0x60: { LD_R_R(H, B);     } case 0x61: { LD_R_R(H, C);        } case 0x62: { LD_R_R(H, D);     } case 0x63: { LD_R_R(H, E);     } case 0x64: { LD_R_R(H, H);     } case 0x65: { LD_R_R(H, L);     } case 0x66: { LD_R_XR16(H, HL); } case 0x67: { LD_R_R(H, A);     }
        case 0x68: { LD_R_R(L, B);     } case 0x69: { LD_R_R(L, C);        } case 0x6A: { LD_R_R(L, D);     } case 0x6B: { LD_R_R(L, E);     } case 0x6C: { LD_R_R(L, H);     } case 0x6D: { LD_R_R(L, L);     } case 0x6E: { LD_R_XR16(L, HL); } case 0x6F: { LD_R_R(L, A);     }
        case 0x70: { LD_XR16_R(HL, B); } case 0x71: { LD_XR16_R(HL, C);    } case 0x72: { LD_XR16_R(HL, D); } case 0x73: { LD_XR16_R(HL, E); } case 0x74: { LD_XR16_R(HL, H); } case 0x75: { LD_XR16_R(HL, L); } case 0x76: { HALT();           } case 0x77: { LD_XR16_R(HL, A); }
        case 0x78: { LD_R_R(A, B);     } case 0x79: { LD_R_R(A, C);        } case 0x7A: { LD_R_R(A, D);     } case 0x7B: { LD_R_R(A, E);     } case 0x7C: { LD_R_R(A, H);     } case 0x7D: { LD_R_R(A, L);     } case 0x7E: { LD_R_XR16(A, HL); } case 0x7F: { LD_R_R(A, A);     }
        case 0x80: { ADD(B);           } case 0x81: { ADD(C);              } case 0x82: { ADD(D);           } case 0x83: { ADD(E);           } case 0x84: { ADD(H);           } case 0x85: { ADD(L);           } case 0x86: { ADD_HL();         }   case 0x87: { ADD(A);         }
        case 0x88: { ADC(B);           } case 0x89: { ADC(C);              } case 0x8A: { ADC(D);           } case 0x8B: { ADC(E);           } case 0x8C: { ADC(H);           } case 0x8D: { ADC(L);           } case 0x8E: { ADC_HL();         }   case 0x8F: { ADC(A);         }
        case 0x90: { SUB(B);           } case 0x91: { SUB(C);              } case 0x92: { SUB(D);           } case 0x93: { SUB(E);           } case 0x94: { SUB(H);           } case 0x95: { SUB(L);           } case 0x96: { SUB_HL();         }   case 0x97: { SUB(A);         }
        case 0x98: { SBC(B);           } case 0x99: { SBC(C);              } case 0x9A: { SBC(D);           } case 0x9B: { SBC(E);           } case 0x9C: { SBC(H);           } case 0x9D: { SBC(L);           } case 0x9E: { SBC_HL();         }   case 0x9F: { SBC(A);         }
        case 0xA0: { AND(B);           } case 0xA1: { AND(C);              } case 0xA2: { AND(D);           } case 0xA3: { AND(E);           } case 0xA4: { AND(H);           } case 0xA5: { AND(L);           } case 0xA6: { AND_XHL();        }   case 0xA7: { AND(A);         }
        case 0xA8: { XOR(B);           } case 0xA9: { XOR(C);              } case 0xAA: { XOR(D);           } case 0xAB: { XOR(E);           } case 0xAC: { XOR(H);           } case 0xAD: { XOR(L);           } case 0xAE: { XOR_XHL();        }   case 0xAF: { XOR(A);         }
        case 0xB0: { OR(B);            } case 0xB1: { OR(C);               } case 0xB2: { OR(D);            } case 0xB3: { OR(E);            } case 0xB4: { OR(H);            } case 0xB5: { OR(L);            } case 0xB6: { OR_XHL();         }   case 0xB7: { OR(A);          }
        case 0xB8: { CP(B);            } case 0xB9: { CP(C);               } case 0xBA: { CP(D);            } case 0xBB: { CP(E);            } case 0xBC: { CP(H);            } case 0xBD: { CP(L);            } case 0xBE: { CP_XHL();         }   case 0xBF: { CP(A);          }
        case 0xC0: { RETNZ();          } case 0xC1: { POP(BC);             } case 0xC2: { JPNZ_U16();       } case 0xC3: { JP_U16();         } case 0xC4: { CALLNZ();         } case 0xC5: { PUSH(BC);         } case 0xC6: { ADD_U8();         } case 0xC7: { RST();            }
        case 0xC8: { RETZ();           } case 0xC9: { RET();               } case 0xCA: { JPZ_U16();        } case 0xCB: { CB();             } case 0xCC: { CALLZ();          } case 0xCD: { CALL();           } case 0xCE: { ADC_U8();         } case 0xCF: { RST();            }
        case 0xD0: { RETNC();          } case 0xD1: { POP(DE);             } case 0xD2: { JPNC_U16();       } case 0xD3: { INV();            } case 0xD4: { CALLNC();         } case 0xD5: { PUSH(DE);         } case 0xD6: { SUB_U8();         } case 0xD7: { RST();            }
        case 0xD8: { RETC();           } case 0xD9: { RETI();              } case 0xDA: { JPC_U16();        } case 0xDB: { INV();            } case 0xDC: { CALLC();          } case 0xDD: { INV();            } case 0xDE: { SBC_U8();         } case 0xDF: { RST();            }
        case 0xE0: { LD_XU8_A();       } case 0xE1: { POP(HL);             } case 0xE2: { LD_XC_A();        } case 0xE3: { INV();            } case 0xE4: { INV();            } case 0xE5: { PUSH(HL);         } case 0xE6: { AND_U8();         } case 0xE7: { RST();            }
        case 0xE8: { ADD_SP_S8();      } case 0xE9: { JP_HL();             } case 0xEA: { LD_XU16_A();      } case 0xEB: { INV();            } case 0xEC: { INV();            } case 0xED: { INV();            } case 0xEE: { XOR_U8();         } case 0xEF: { RST();            }
        case 0xF0: { LD_A_XU8();       } case 0xF1: { POP_AF();            } case 0xF2: { LD_A_XC();        } case 0xF3: { DI();             } case 0xF4: { INV();            } case 0xF5: { PUSH(AF);         } case 0xF6: { OR_U8();          } case 0xF7: { RST();            }
        case 0xF8: { LD_HL_SP_S8();    } case 0xF9: { LD_R16_R16(SP, HL);  } case 0xFA: { LD_A_XU16();      } case 0xFB: { EI();             } case 0xFC: { INV();            } case 0xFD: { INV();            } case 0xFE: { CP_U8();          } case 0xFF: { RST();            }
    }

    return -1;
}

int execute_CB(struct cpu* cpu, u8 opcode) {

    switch(opcode) {
        case 0x00: { RLC(B);    } case 0x01: { RLC(C);    } case 0x02: { RLC(D);    } case 0x03: { RLC(E);    } case 0x04: { RLC(H);    } case 0x05: { RLC(L);    } case 0x06: { RLC_HL();  } case 0x07: { RLC(A);    }
        case 0x08: { RRC(B);    } case 0x09: { RRC(C);    } case 0x0A: { RRC(D);    } case 0x0B: { RRC(E);    } case 0x0C: { RRC(H);    } case 0x0D: { RRC(L);    } case 0x0E: { RRC_HL();  } case 0x0F: { RRC(A);    }
        case 0x10: { RL(B);     } case 0x11: { RL(C);     } case 0x12: { RL(D);     } case 0x13: { RL(E);     } case 0x14: { RL(H);     } case 0x15: { RL(L);     } case 0x16: { RL_HL();   } case 0x17: { RL(A);     }
        case 0x18: { RR(B);     } case 0x19: { RR(C);     } case 0x1A: { RR(D);     } case 0x1B: { RR(E);     } case 0x1C: { RR(H);     } case 0x1D: { RR(L);     } case 0x1E: { RR_HL();   } case 0x1F: { RR(A);     }
        case 0x20: { SLA(B);    } case 0x21: { SLA(C);    } case 0x22: { SLA(D);    } case 0x23: { SLA(E);    } case 0x24: { SLA(H);    } case 0x25: { SLA(L);    } case 0x26: { SLA_HL();  } case 0x27: { SLA(A);    }
        case 0x28: { SRA(B);    } case 0x29: { SRA(C);    } case 0x2A: { SRA(D);    } case 0x2B: { SRA(E);    } case 0x2C: { SRA(H);    } case 0x2D: { SRA(L);    } case 0x2E: { SRA_HL();  } case 0x2F: { SRA(A);    }
        case 0x30: { SWAP(B);   } case 0x31: { SWAP(C);   } case 0x32: { SWAP(D);   } case 0x33: { SWAP(E);   } case 0x34: { SWAP(H);   } case 0x35: { SWAP(L);   } case 0x36: { SWAP_HL(); } case 0x37: { SWAP(A);   }
        case 0x38: { SRL(B);    } case 0x39: { SRL(C);    } case 0x3A: { SRL(D);    } case 0x3B: { SRL(E);    } case 0x3C: { SRL(H);    } case 0x3D: { SRL(L);    } case 0x3E: { SRL_HL();  } case 0x3F: { SRL(A);    }
        case 0x40: { BIT(0, B); } case 0x41: { BIT(0, C); } case 0x42: { BIT(0, D); } case 0x43: { BIT(0, E); } case 0x44: { BIT(0, H); } case 0x45: { BIT(0, L); } case 0x46: { BIT_HL(0); } case 0x47: { BIT(0, A); }
        case 0x48: { BIT(1, B); } case 0x49: { BIT(1, C); } case 0x4A: { BIT(1, D); } case 0x4B: { BIT(1, E); } case 0x4C: { BIT(1, H); } case 0x4D: { BIT(1, L); } case 0x4E: { BIT_HL(1); } case 0x4F: { BIT(1, A); }
        case 0x50: { BIT(0, B); } case 0x51: { BIT(2, C); } case 0x52: { BIT(2, D); } case 0x53: { BIT(2, E); } case 0x54: { BIT(2, H); } case 0x55: { BIT(2, L); } case 0x56: { BIT_HL(2); } case 0x57: { BIT(2, A); }
        case 0x58: { BIT(3, B); } case 0x59: { BIT(3, C); } case 0x5A: { BIT(3, D); } case 0x5B: { BIT(3, E); } case 0x5C: { BIT(3, H); } case 0x5D: { BIT(3, L); } case 0x5E: { BIT_HL(3); } case 0x5F: { BIT(3, A); }
        case 0x60: { BIT(4, B); } case 0x61: { BIT(4, C); } case 0x62: { BIT(4, D); } case 0x63: { BIT(4, E); } case 0x64: { BIT(4, H); } case 0x65: { BIT(4, L); } case 0x66: { BIT_HL(4); } case 0x67: { BIT(4, A); }
        case 0x68: { BIT(5, B); } case 0x69: { BIT(5, C); } case 0x6A: { BIT(5, D); } case 0x6B: { BIT(5, E); } case 0x6C: { BIT(5, H); } case 0x6D: { BIT(5, L); } case 0x6E: { BIT_HL(5); } case 0x6F: { BIT(5, A); }
        case 0x70: { BIT(6, B); } case 0x71: { BIT(6, C); } case 0x72: { BIT(6, D); } case 0x73: { BIT(6, E); } case 0x74: { BIT(6, H); } case 0x75: { BIT(6, L); } case 0x76: { BIT_HL(6); } case 0x77: { BIT(6, A); }
        case 0x78: { BIT(7, B); } case 0x79: { BIT(7, C); } case 0x7A: { BIT(7, D); } case 0x7B: { BIT(7, E); } case 0x7C: { BIT(7, H); } case 0x7D: { BIT(7, L); } case 0x7E: { BIT_HL(7); } case 0x7F: { BIT(7, A); }
        case 0x80: { RES(0, B); } case 0x81: { RES(0, C); } case 0x82: { RES(0, D); } case 0x83: { RES(0, E); } case 0x84: { RES(0, H); } case 0x85: { RES(0, L); } case 0x86: { RES_HL(0); } case 0x87: { RES(0, A); }
        case 0x88: { RES(1, B); } case 0x89: { RES(1, C); } case 0x8A: { RES(1, D); } case 0x8B: { RES(1, E); } case 0x8C: { RES(1, H); } case 0x8D: { RES(1, L); } case 0x8E: { RES_HL(1); } case 0x8F: { RES(1, A); }
        case 0x90: { RES(0, B); } case 0x91: { RES(2, C); } case 0x92: { RES(2, D); } case 0x93: { RES(2, E); } case 0x94: { RES(2, H); } case 0x95: { RES(2, L); } case 0x96: { RES_HL(2); } case 0x97: { RES(2, A); }
        case 0x98: { RES(3, B); } case 0x99: { RES(3, C); } case 0x9A: { RES(3, D); } case 0x9B: { RES(3, E); } case 0x9C: { RES(3, H); } case 0x9D: { RES(3, L); } case 0x9E: { RES_HL(3); } case 0x9F: { RES(3, A); }
        case 0xA0: { RES(4, B); } case 0xA1: { RES(4, C); } case 0xA2: { RES(4, D); } case 0xA3: { RES(4, E); } case 0xA4: { RES(4, H); } case 0xA5: { RES(4, L); } case 0xA6: { RES_HL(4); } case 0xA7: { RES(4, A); }
        case 0xA8: { RES(5, B); } case 0xA9: { RES(5, C); } case 0xAA: { RES(5, D); } case 0xAB: { RES(5, E); } case 0xAC: { RES(5, H); } case 0xAD: { RES(5, L); } case 0xAE: { RES_HL(5); } case 0xAF: { RES(5, A); }
        case 0xB0: { RES(6, B); } case 0xB1: { RES(6, C); } case 0xB2: { RES(6, D); } case 0xB3: { RES(6, E); } case 0xB4: { RES(6, H); } case 0xB5: { RES(6, L); } case 0xB6: { RES_HL(6); } case 0xB7: { RES(6, A); }
        case 0xB8: { RES(7, B); } case 0xB9: { RES(7, C); } case 0xBA: { RES(7, D); } case 0xBB: { RES(7, E); } case 0xBC: { RES(7, H); } case 0xBD: { RES(7, L); } case 0xBE: { RES_HL(7); } case 0xBF: { RES(7, A); }
        case 0xC0: { SET(0, B); } case 0xC1: { SET(0, C); } case 0xC2: { SET(0, D); } case 0xC3: { SET(0, E); } case 0xC4: { SET(0, H); } case 0xC5: { SET(0, L); } case 0xC6: { SET_HL(0); } case 0xC7: { SET(0, A); }
        case 0xC8: { SET(1, B); } case 0xC9: { SET(1, C); } case 0xCA: { SET(1, D); } case 0xCB: { SET(1, E); } case 0xCC: { SET(1, H); } case 0xCD: { SET(1, L); } case 0xCE: { SET_HL(1); } case 0xCF: { SET(1, A); }
        case 0xD0: { SET(0, B); } case 0xD1: { SET(2, C); } case 0xD2: { SET(2, D); } case 0xD3: { SET(2, E); } case 0xD4: { SET(2, H); } case 0xD5: { SET(2, L); } case 0xD6: { SET_HL(2); } case 0xD7: { SET(2, A); }
        case 0xD8: { SET(3, B); } case 0xD9: { SET(3, C); } case 0xDA: { SET(3, D); } case 0xDB: { SET(3, E); } case 0xDC: { SET(3, H); } case 0xDD: { SET(3, L); } case 0xDE: { SET_HL(3); } case 0xDF: { SET(3, A); }
        case 0xE0: { SET(4, B); } case 0xE1: { SET(4, C); } case 0xE2: { SET(4, D); } case 0xE3: { SET(4, E); } case 0xE4: { SET(4, H); } case 0xE5: { SET(4, L); } case 0xE6: { SET_HL(4); } case 0xE7: { SET(4, A); }
        case 0xE8: { SET(5, B); } case 0xE9: { SET(5, C); } case 0xEA: { SET(5, D); } case 0xEB: { SET(5, E); } case 0xEC: { SET(5, H); } case 0xED: { SET(5, L); } case 0xEE: { SET_HL(5); } case 0xEF: { SET(5, A); }
        case 0xF0: { SET(6, B); } case 0xF1: { SET(6, C); } case 0xF2: { SET(6, D); } case 0xF3: { SET(6, E); } case 0xF4: { SET(6, H); } case 0xF5: { SET(6, L); } case 0xF6: { SET_HL(6); } case 0xF7: { SET(6, A); }
        case 0xF8: { SET(7, B); } case 0xF9: { SET(7, C); } case 0xFA: { SET(7, D); } case 0xFB: { SET(7, E); } case 0xFC: { SET(7, H); } case 0xFD: { SET(7, L); } case 0xFE: { SET_HL(7); } case 0xFF: { SET(7, A); }
    }

    return -1;
}
