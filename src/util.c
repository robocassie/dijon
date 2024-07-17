#include <stdio.h>
#include <string.h>

#include "common.h"
#include "instructions.h"
#include "gb.h"
#include "cpu.h"


void log_instruction_line(struct cpu* cpu, u16 addr, u16 opcode) {
    // Longest instruction ("LD (xxxx), SP  ", 15 chars) + longest possible operand (4 chars) + 1 byte space
    int maxLen = 15 + 4 + 1;
    char disasmStr[40];

    // Alternate blue and black lines
    if(cpu->linesPrinted % 2 == 0) {
        printf("\033[;44m");
    }
    else {
        printf("\033[;40m");
    }
    cpu->linesPrinted++;

    // If instruction is PREFIX CB, make it red
    if(opcode == 0xCB) {
        sprintf(disasmStr, "\033[31m%s", instructions[opcode].disasm);
    }
    else {
        sprintf(disasmStr, "%s", instructions[opcode].disasm);
    }

    u16 adjOpcode = opcode;
    if(adjOpcode > 256)
        adjOpcode -= 256;

    if(instructions[opcode].len == 1) {
        printf("\033[33m%04X: \033[36m%02X      \033[37m %s",
                addr, (u8)adjOpcode, disasmStr);
    }
    else if(instructions[opcode].len == 2) {
        u8 operand = gb_read8(cpu->gb, addr + 1);
        printf("\033[33m%04X: \033[36m%02X %02X   \033[37m %s\033[32m%02X",
                addr, (u8)adjOpcode, operand, disasmStr, operand);
    }
    else {
        u16 operand = gb_read16(cpu->gb, addr + 1);
        printf("\033[33m%04X: \033[36m%02X %02X %02X\033[37m %s\033[32m%04X",
                addr, (u8)adjOpcode, (operand & 0xFF), (operand >> 8), disasmStr, operand);
    }
    // Add padding to the end of the disasm so the registers are evenly aligned
    int instrLen = instructions[opcode].len;
    int operandLen = 0;
    if(instrLen == 2) {
        operandLen = 2;
    }
    else if(instrLen == 3) {
        operandLen = 4;
    }

    // Align the registers by adding appropriate padding after each instruction
    int paddingLen = maxLen - (strlen(instructions[opcode].disasm) + operandLen);
    for(int i = 0; i < paddingLen; i++) {
        printf(" ");
    }
    // Print the registers
    printf("\033[37mA:\033[35m%02X\033[37m B:\033[35m%02X\033[37m C:\033[35m%02X\033[37m D:\033[35m%02X\033[37m E:\033[35m%02X\033[37m H:\033[35m%02X\033[37m L:\033[35m%02X\033[37m F:\033[35m%02X\033[37m SP:\033[35m%04X\033[0m\n",
            cpu->a, cpu->b, cpu->c, cpu->d, cpu->e, cpu->h, cpu->l, cpu->f, cpu->sp);
}
