#include <stdio.h>
#include <string.h>

#include "mbc.h"

struct mbc mbcs[2];

void initMBCs() {
    memset(mbcs[0].regs, 0x00, 4);
    mbcs[0].write8 = &mbc0_write8;
    mbcs[0].write16 = &mbc0_write16;
    mbcs[0].read8 = &mbc0_read8;
    mbcs[0].read16 = &mbc0_read16;

    // All registers default to 00
    memset(mbcs[1].regs, 0x00, 4);
    mbcs[1].write8 = &mbc1_write8;
    mbcs[1].write16 = &mbc1_write16;
    mbcs[1].read8 = &mbc1_read8;
    mbcs[1].read16 = &mbc1_read16;
}

/***********
 ** MBC 0 **
************/
void mbc0_write8(struct cart_t* cart, u16 addr, u8 v) {
    return;
}

void mbc0_write16(struct cart_t* cart, u16 addr, u16 v) {
    return;
}

u8 mbc0_read8(struct cart_t* cart, u16 addr) {
    return cart->rom[addr & 0x7FFF];
}

u16 mbc0_read16(struct cart_t* cart, u16 addr) {
    u8 topByte = 0x00;
    if(addr == 0x7FFF) {
        printf("MBC0: Reading 16-bit value at 0x7FFF. Using 0xFF as the top byte!\n");
        topByte = 0xFF;
    } else {
        topByte = cart->rom[(addr + 1) & 0x7FFF];
    }
    return cart->rom[addr & 0x7FFF] | topByte << 8;
}

/***********
 ** MBC 1 **
************/
void mbc1_write8(struct cart_t* cart, u16 addr, u8 v) {
    if(addr >= 0x0000 && addr < 0x2000) {
        if((v & 0xF) == 0xA) {
            printf("MBC1: Enabling RAM\n");
            cart->mbc->regs[MBC1_RAMENABLE] = 1;
        } else {
            printf("MBC1: Disabling RAM\n");
            cart->mbc->regs[MBC1_RAMENABLE] = 0;
        }
    }
    else if(addr >= 0x2000 && addr < 0x4000) {
        // Mask to only the number of bits needed
        // by the ROM size, max 0x1F
        u8 romSize = cart->romSize;
        if(romSize > 4)
            romSize = 4;
        u8 mask = (1 << (romSize + 1)) - 1;
        u8 bank = v & mask;
        if(cart->romSize >= 5)
            bank = (cart->mbc->regs[MBC1_RAMBANK] << 5) | bank;
        // A quirk here is that if the rom size is <= 256KB
        // you can map bank 0 to 0x4000-7FFF, because this
        // check masks using the full 5 bits rather than the mask
        // calculated above when actually setting the bank
        // This also ignores the upper two bits from the ram bank
        // register, so banks 20, 40, and 60 trip this check and
        // become 21, 41, and 61, respectively.
        if((v & 0x1F) == 0x00)
            bank++;
        cart->mbc->regs[MBC1_ROMBANK] = bank;
    }
    else if(addr >= 0x4000 && addr < 0x6000) {
        if(cart->romSize < 5 && cart->ramSize < 3) {
            // If ROM size is less than 1MB and RAM
            // size is less than 32KB then do nothing
            return;
        } else {
            cart->mbc->regs[MBC1_RAMBANK] = v & 0x3;
        }
    }
}

void mbc1_write16(struct cart_t* cart, u16 addr, u16 v) {
    return;
}

u8 mbc1_read8(struct cart_t* cart, u16 addr) {
    addr &= 0x7FFF;
    if(addr >= 0x0000 && addr < 0x4000) {
        return cart->rom[addr];
    }
    else if(addr >= 0x4000 && addr < 0x8000) {
        u8 bank = cart->mbc->regs[MBC1_ROMBANK];
        u32 offs = bank * 0x4000 + (addr - 0x4000);
        return cart->rom[offs];
    }
    return 0xFF;
}

u16 mbc1_read16(struct cart_t* cart, u16 addr) {
    addr &= 0x7FFF;
    u8 topByte = 0x00;
    u8 bottomByte = 0x00;
    if(addr >= 0x0000 && addr < 0x4000) {
        if(addr == 0x3FFF) {
            printf("MBC1: Reading 16-bit value at the edge of a bank. Using 0xFF as the top byte!\n");
            topByte = 0xFF;
        } else {
            topByte = cart->rom[addr + 1];
        }
        bottomByte = cart->rom[addr];
    }
    else if(addr >= 0x4000 && addr < 0x8000) {
        u8 bank = cart->mbc->regs[MBC1_ROMBANK];
        u32 offs = bank * 0x4000 + (addr - 0x4000);

        if(addr == 0x7FFF) {
            printf("MBC1: Reading 16-bit value at the edge of a bank. Using 0xFF as the top byte!\n");
            topByte = 0xFF;
        } else {
            topByte = cart->rom[offs + 1];
        }
        bottomByte = cart->rom[offs];
    }

    return bottomByte | (topByte << 8);
}
