#pragma once
#include "common.h"

struct cpu;

void log_instruction_line(struct cpu* cpu, u16 addr, u16 opcode);
