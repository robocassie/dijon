#pragma once
#include "common.h"

struct cpu;

extern struct instr_t {
    int (*ptr)(struct cpu*, u8);
    int len;
    const char* disasm;
} instructions[512];


int und(struct cpu* cpu, u8 op);
int nop(struct cpu* cpu, u8 op);
int stop(struct cpu* cpu, u8 op);
int halt(struct cpu* cpu, u8 op);
int ei(struct cpu* cpu, u8 op);
int di(struct cpu* cpu, u8 op);
int cb(struct cpu* cpu, u8 op);
int inc(struct cpu* cpu, u8 op);
int dec(struct cpu* cpu, u8 op);
int add(struct cpu* cpu, u8 op);
int adc(struct cpu* cpu, u8 op);
int sub(struct cpu* cpu, u8 op);
int sbc(struct cpu* cpu, u8 op);
int ld(struct cpu* cpu, u8 op);
int ldh(struct cpu* cpu, u8 op);
int ldi(struct cpu* cpu, u8 op);
int ldd(struct cpu* cpu, u8 op);
int jp(struct cpu* cpu, u8 op);
int jr(struct cpu* cpu, u8 op);
int call(struct cpu* cpu, u8 op);
int ret(struct cpu* cpu, u8 op);
int reti(struct cpu* cpu, u8 op);
int push(struct cpu* cpu, u8 op);
int pop(struct cpu* cpu, u8 op);
int rst(struct cpu* cpu, u8 op);
int scf(struct cpu* cpu, u8 op);
int ccf(struct cpu* cpu, u8 op);
int cpl(struct cpu* cpu, u8 op);
int rlca(struct cpu* cpu, u8 op);
int rla(struct cpu* cpu, u8 op);
int rrca(struct cpu* cpu, u8 op);
int rra(struct cpu* cpu, u8 op);
int _and(struct cpu* cpu, u8 op);
int _xor(struct cpu* cpu, u8 op);
int _or(struct cpu* cpu, u8 op);
int cp(struct cpu* cpu, u8 op);
int daa(struct cpu* cpu, u8 op);
// CB Opcodes
int rlc(struct cpu* cpu, u8 op);
int rl(struct cpu* cpu, u8 op);
int rrc(struct cpu* cpu, u8 op);
int rr(struct cpu* cpu, u8 op);
int sla(struct cpu* cpu, u8 op);
int sra(struct cpu* cpu, u8 op);
int srl(struct cpu* cpu, u8 op);
int swap(struct cpu* cpu, u8 op);
int bit(struct cpu* cpu, u8 op);
int res(struct cpu* cpu, u8 op);
int set(struct cpu* cpu, u8 op);
