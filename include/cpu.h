#pragma once
#include "common.h"

#define INTERRUPT_VBLANK 0x01
#define INTERRUPT_STAT   0x02
#define INTERRUPT_TIMER  0x04
#define INTERRUPT_SERIAL 0x08
#define INTERRUPT_JOYPAD 0x10

struct gb;

struct cpu {
    union {
        u16 bc;
        struct {
            u8 c;
            u8 b;
        };
    };
    union {
        u16 de;
        struct {
            u8 e;
            u8 d;
        };
    };
    union {
        u16 hl;
        struct {
            u8 l;
            u8 h;
        };
    };
    union {
        u16 af;
        struct {

            union {
                u8 f;
                struct {
                    u8 f_x : 4;
                    u8 f_c : 1;
                    u8 f_h : 1;
                    u8 f_n : 1;
                    u8 f_z : 1;
                };
            };
            u8 a;

        };
    };

    u16 pc, sp;

    bool ime;
    int imeWait;

    bool stopped;
    bool paused;
    bool stopAtBootrom;
    bool loggingEnabled;
    u64 linesPrinted;
    int lastCycles;
    struct gb* gb;
};

void cpu_init(struct cpu*, struct gb* gb);
void cpu_destroy(struct cpu*);
void cpu_reset(struct cpu*);

void cpu_request_interrupt(struct cpu*, u8 mask);

void cpu_set_logging_enabled(struct cpu*, bool e);
void cpu_set_stop_at_bootrom(struct cpu*, bool e);

int cpu_run(struct cpu*);
int cpu_execute(struct cpu*);
