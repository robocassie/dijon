#pragma once
#include "common.h"

struct cpu;

extern struct instr_t {
    int len;
    const char* disasm;
} instructions[512];

int execute_instr(struct cpu* cpu);
int execute_CB(struct cpu* cpu, u8 opcode);
