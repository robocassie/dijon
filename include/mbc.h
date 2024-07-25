#pragma once
#include "common.h"

extern struct mbc {
    u8 regs[4];
    u8* rom;
    u8* upperBank_ptr;

    void ((*write8)(struct mbc*, u16, u8));
    void ((*write16)(struct mbc*, u16, u16));
    u8   ((*read8)(struct mbc*, u16));
    u16  ((*read16)(struct mbc*, u16));
} mbcs[2];

void initMBCs();

void mbc0_write8(struct mbc* mbc, u16 addr, u8 v);
void mbc0_write16(struct mbc* mbc, u16 addr, u16 v);
u8   mbc0_read8(struct mbc* mbc, u16 addr);
u16  mbc0_read16(struct mbc* mbc, u16 addr);

#define MBC1_RAMENABLE  0
#define MBC1_ROMBANK    1
#define MBC1_RAMBANK    2
#define MBC1_ROMBANKTOP 2
#define MBC1_MODE       3
void mbc1_write8(struct mbc* mbc, u16 addr, u8 v);
void mbc1_write16(struct mbc* mbc, u16 addr, u16 v);
u8   mbc1_read8(struct mbc* mbc, u16 addr);
u16  mbc1_read16(struct mbc* mbc, u16 addr);