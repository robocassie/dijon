#include <stdio.h>
#include <string.h>

#include "mbc.h"

struct mbc mbcs[2];

void initMBCs() {
    memset(mbcs[0].regs, 0x00, 4);
    mbcs[0].rom = NULL;
    mbcs[0].upperBank_ptr = NULL;
    mbcs[0].write8 = &mbc0_write8;
    mbcs[0].write16 = &mbc0_write16;
    mbcs[0].read8 = &mbc0_read8;
    mbcs[0].read16 = &mbc0_read16;

    memset(mbcs[1].regs, 0x00, 4);
    mbcs[1].rom = NULL;
    mbcs[1].upperBank_ptr = NULL;
    mbcs[1].write8 = &mbc1_write8;
    mbcs[1].write16 = &mbc1_write16;
    mbcs[1].read8 = &mbc1_read8;
    mbcs[1].read16 = &mbc1_read16;
}

/***********
 ** MBC 0 **
************/
void mbc0_write8(struct mbc* mbc, u16 addr, u8 v) {
    return;
}

void mbc0_write16(struct mbc* mbc, u16 addr, u16 v) {
    return;
}

u8 mbc0_read8(struct mbc* mbc, u16 addr) {
    return mbc->rom[addr & 0x7FFF];
}

u16 mbc0_read16(struct mbc* mbc, u16 addr) {
    u8 topByte = 0x00;
    if(addr == 0x7FFF) {
        printf("MBC0: Reading 16-bit value at 0x7FFF. Using 0xFF as the top byte!\n");
        topByte = 0xFF;
    } else {
        topByte = mbc->rom[(addr + 1) & 0x7FFF];
    }
    return mbc->rom[addr & 0x7FFF] | topByte << 8;
}

/***********
 ** MBC 1 **
************/
void mbc1_write8(struct mbc* mbc, u16 addr, u8 v) {
    if(addr >= 0x0000 && addr < 0x2000) {
        if((v & 0xF) == 0xA) {
            printf("MBC1: Enabling RAM\n");
            mbc->regs[MBC1_RAMENABLE] = 1;
        } else {
            printf("MBC1: Disabling RAM\n");
            mbc->regs[MBC1_RAMENABLE] = 0;
        }
    }
    else if(addr >= 0x2000 && addr < 0x4000) {
        // TODO: Use less bits for smaller ROMS
        mbc->regs[MBC1_ROMBANK] = v & 0x1F;
    }
}

void mbc1_write16(struct mbc* mbc, u16 addr, u16 v) {
    return;
}

u8 mbc1_read8(struct mbc* mbc, u16 addr) {
    addr &= 0x7FFF;
    if(addr >= 0x0000 && addr < 0x4000) {
        return mbc->rom[addr];
    }
    else if(addr >= 0x4000 && addr < 0x8000) {
        u8 bank = mbc->regs[MBC1_ROMBANK];
        if(bank == 0x00) bank = 0x01;
        u32 offs = bank * 0x4000 + (addr - 0x4000);
        return mbc->rom[offs];
    }
    return 0xFF;
}

u16 mbc1_read16(struct mbc* mbc, u16 addr) {
    addr &= 0x7FFF;
    u8 topByte = 0x00;
    u8 bottomByte = 0x00;
    if(addr >= 0x0000 && addr < 0x4000) {
        if(addr == 0x3FFF) {
            printf("MBC1: Reading 16-bit value at the edge of a bank. Using 0xFF as the top byte!\n");
            topByte = 0xFF;
        } else {
            topByte = mbc->rom[addr + 1];
        }
        bottomByte = mbc->rom[addr];
    }
    else if(addr >= 0x4000 && addr < 0x8000) {
        u8 bank = mbc->regs[MBC1_ROMBANK];
        if(bank == 0x00) bank = 0x01;
        u32 offs = bank * 0x4000 + (addr - 0x4000);

        if(addr == 0x7FFF) {
            printf("MBC1: Reading 16-bit value at the edge of a bank. Using 0xFF as the top byte!\n");
            topByte = 0xFF;
        } else {
            topByte = mbc->rom[offs + 1];
        }
        bottomByte = mbc->rom[offs];
    }

    return bottomByte | (topByte << 8);
}