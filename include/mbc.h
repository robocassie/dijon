#pragma once
#include "common.h"

struct cart_t;

extern struct mbc {
    u8 regs[4];

    void (*write8)(struct cart_t*, u16, u8);
    void (*write16)(struct cart_t*, u16, u16);
    u8   (*read8)(struct cart_t*, u16);
    u16  (*read16)(struct cart_t*, u16);
} mbcs[4];

struct cart_t {
    u8* rom;
    struct mbc* mbc;

    u8 mbcCode;
    u8 romSize;
    u8 ramSize;
};

void initMBCs();

void mbc0_write8(struct cart_t* cart, u16 addr, u8 v);
void mbc0_write16(struct cart_t* cart, u16 addr, u16 v);
u8   mbc0_read8(struct cart_t* cart, u16 addr);
u16  mbc0_read16(struct cart_t* cart, u16 addr);

#define MBC1_RAMENABLE  0
#define MBC1_ROMBANK    1
#define MBC1_RAMBANK    2
#define MBC1_MODE       3
void mbc1_write8(struct cart_t* cart, u16 addr, u8 v);
void mbc1_write16(struct cart_t* cart, u16 addr, u16 v);
u8   mbc1_read8(struct cart_t* cart, u16 addr);
u16  mbc1_read16(struct cart_t* cart, u16 addr);

// TODO: More registers
#define MBC3_ROMBANK    0
void mbc3_write8(struct cart_t* cart, u16 addr, u8 v);
void mbc3_write16(struct cart_t* cart, u16 addr, u16 v);
u8   mbc3_read8(struct cart_t* cart, u16 addr);
u16  mbc3_read16(struct cart_t* cart, u16 addr);
