#pragma once
#include <stdio.h>

#include "common.h"
#include "mbc.h"

struct cpu;
struct ppu;

#define P1_SELECT_BUTTONS   0x20
#define P1_SELECT_DPAD      0x10
#define P1_KEY_START        0x08
#define P1_KEY_SELECT       0x04
#define P1_KEY_B            0x02
#define P1_KEY_A            0x01
#define P1_KEY_DOWN         0x08
#define P1_KEY_UP           0x04
#define P1_KEY_LEFT         0x02
#define P1_KEY_RIGHT        0x01

enum key_e {
    GB_KEY_START  = 0x80,
    GB_KEY_SELECT = 0x40,
    GB_KEY_B      = 0x20,
    GB_KEY_A      = 0x10,
    GB_KEY_DOWN   = 0x08,
    GB_KEY_UP     = 0x04,
    GB_KEY_LEFT   = 0x02,
    GB_KEY_RIGHT  = 0x01
};

struct gb {
    struct cpu* cpu;
    struct ppu* ppu;

    u8* mmap;
    u8* bootrom;

    struct cart_t cart;

    u8 keysPressed;

    bool inBootrom;
    bool dmaScheduled;
    bool inDMA;
    int dmaCycles;
    u16 dmaAddress;
};

void gb_init(struct gb*);
int gb_run(struct gb*, bool* stopped, bool* frameCompleted);
void gb_destroy(struct gb*);
void gb_init_mmap(struct gb*);
void gb_readBootrom(struct gb*, FILE *bootrom);
void gb_readRom(struct gb*, FILE* rom);

void gb_dma(struct gb* gb);
void gb_keypress(struct gb* gb, enum key_e key);
void gb_keyrelease(struct gb* gb, enum key_e key);

u8* gb_get_mmap_ptr(struct gb*, u16 addr);
u8 gb_read8(struct gb*, u16 addr);
void gb_write8(struct gb*, u16 addr, u8 byte);
u16 gb_read16(struct gb*, u16 addr);
void gb_write16(struct gb*, u16 addr, u16 word);
