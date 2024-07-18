#include "cpu.h"
#include "gb.h"
#include "instructions.h"


void cpu_init(struct cpu* cpu, struct gb* gb) {
    cpu->gb = gb;

    cpu_reset(cpu);
}

void cpu_destroy(struct cpu* cpu) {

}

void cpu_reset(struct cpu* cpu) {
    cpu->pc = 0x0000;

    cpu->ime = false;
    cpu->imeWait = -1;

    cpu->linesPrinted = 0;
}

void cpu_request_interrupt(struct cpu* cpu, u8 mask) {
    u8 oldIF = gb_read8(cpu->gb, 0xFF0F);
    gb_write8(cpu->gb, 0xFF0F, oldIF | mask);
}

bool cpu_service_interrupts(struct cpu* cpu) {

    u8 enabledInterrupts = gb_read8(cpu->gb, 0xFFFF);
    u8 requestedInterrupts = gb_read8(cpu->gb, 0xFF0F);
    u8 interruptsToDo = enabledInterrupts & requestedInterrupts;

    u8 interruptToHandle = 0x00;
    u16 vectorAddress = 0x0000;

    // These are checked in order of priority
    if(interruptsToDo & INTERRUPT_VBLANK) {
        interruptToHandle = INTERRUPT_VBLANK;
        vectorAddress = 0x0040;
    } else if(interruptsToDo & INTERRUPT_STAT) {
        interruptToHandle = INTERRUPT_STAT;
        vectorAddress = 0x0048;
    } else if(interruptsToDo & INTERRUPT_TIMER) {
        interruptToHandle = INTERRUPT_TIMER;
        vectorAddress = 0x0050;
    } else if(interruptsToDo & INTERRUPT_SERIAL) {
        interruptToHandle = INTERRUPT_SERIAL;
        vectorAddress = 0x0058;
    } else if(interruptsToDo & INTERRUPT_JOYPAD) {
        interruptToHandle = INTERRUPT_JOYPAD;
        vectorAddress = 0x0060;
    }

    if(interruptToHandle != 0x00) {
        gb_write8(cpu->gb, 0xFF0F, requestedInterrupts & (~interruptToHandle));
        cpu->ime = false;
        u16 returnAddr = cpu->pc;
        gb_write8(cpu->gb, --cpu->sp, (returnAddr & 0xFF00) >> 8);
        gb_write8(cpu->gb, --cpu->sp, returnAddr & 0xFF);
        cpu->pc = vectorAddress;
        cpu->lastCycles = 20;
        return true;
    }

    return false;
}

void cpu_set_logging_enabled(struct cpu* cpu, bool e) {
    cpu->loggingEnabled = e;
}

void cpu_set_stop_at_bootrom(struct cpu* cpu, bool e) {
    cpu->stopAtBootrom = e;
}

int cpu_run(struct cpu* cpu) {
    if(cpu->paused || (cpu->stopAtBootrom && cpu->pc > 0xFF)) {
        return 0;
    }

    if(cpu->imeWait > 0) {
        cpu->imeWait--;
    }

    int instrCycles = execute_instr(cpu);
    if(instrCycles == -1) {
        return -1;
    }
    cpu->lastCycles = instrCycles;

    // EI delays enabling ime by one instruction
    if(cpu->imeWait == 0) {
        cpu->imeWait = -1;
        cpu->ime = true;
    }
    if(cpu->ime) {
        cpu_service_interrupts(cpu);
    }

    return 0;
}
