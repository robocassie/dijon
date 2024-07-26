#include "gb.h"

#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "ppu.h"
#include "mbc.h"


void gb_init(struct gb* gb) {

    printf("Initializing GB...\n");
    
    initMBCs();

    gb_init_mmap(gb);
    gb->cart.rom = NULL;

    gb->cpu = (struct cpu*) malloc(sizeof(struct cpu));
    cpu_init(gb->cpu, gb);
    gb->ppu = (struct ppu*) malloc(sizeof(struct ppu));
    ppu_init(gb->ppu, gb);

    gb->keysPressed = 0xFF;
    gb->inBootrom = true;
}

int gb_run(struct gb* gb, bool* stopped, bool* frameCompleted) {
    if(gb->inDMA) {
        gb->dmaCycles -= gb->cpu->lastCycles;
        if(gb->dmaCycles <= 0) {
            gb_dma(gb);
            gb->inDMA = false;
        }
    }
    if(gb->dmaScheduled) {
        gb->dmaScheduled = false;
        gb->inDMA = true;
    }
    if(cpu_run(gb->cpu) < 0) {
        return -1;
    }
    *frameCompleted = ppu_run(gb->ppu, gb->cpu->lastCycles);

    *stopped = gb->cpu->stopped;

    return 0;
}

void gb_destroy(struct gb* gb) {
    ppu_destroy(gb->ppu);
    free(gb->ppu);
    cpu_destroy(gb->cpu);
    free(gb->cpu);
    free(gb->mmap);
    free(gb->cart.rom);
    free(gb->bootrom);
}

void gb_init_mmap(struct gb* gb) {

    gb->mmap = (u8*) malloc(1024 * 64); // 64 KB of memory total
}

void gb_readBootrom(struct gb* gb, FILE* bootrom) {
    
    gb->bootrom = (u8*) malloc(0x100);
    fread(gb->bootrom, 1, 0x100, bootrom);
}

void gb_readRom(struct gb* gb, FILE* rom) {
    fseek(rom, 0L, SEEK_END);
    long size = ftell(rom);
    rewind(rom);

    gb->cart.rom = (u8*) malloc(size);
    fread(gb->cart.rom, 1, size, rom);

    rewind(rom);

    int bytes_to_copy = (size < 0x8000)? size : 0x8000;
    fread(gb->mmap, 1, bytes_to_copy, rom);

    // Read cartridge header
    gb->cart.mbcCode = gb->cart.rom[0x147];
    printf("ROM size: %ld, MBC: %02X\n", size, gb->cart.mbcCode);
    switch(gb->cart.mbcCode) {
        case 0x00:
            gb->cart.mbc = &mbcs[0];
            break;
        case 0x01:
            gb->cart.mbc = &mbcs[1];
            break;
        case 0x03:
        case 0x13:
            gb->cart.mbc = &mbcs[3];
            break;
        default:
            printf("Warning: Unsupported MBC %02X. Using MBC 1!\n", gb->cart.mbcCode);
            gb->cart.mbc = &mbcs[1];
    }

    // ROM size in KB is 32 * (1 << n),
    // where n is rom[0x148]
    gb->cart.romSize = gb->cart.rom[0x148];
    gb->cart.ramSize = gb->cart.rom[0x149];
}

void gb_disable_bootrom(struct gb* gb) {
    printf("Disabling bootrom!\n");
    gb->inBootrom = false;
}

void gb_dma(struct gb* gb) {
    for(u16 i = 0; i < 0xA0; i++) {
        gb_write8(gb, 0xFE00 + i, gb_read8(gb, gb->dmaAddress + i));
    }
    //printf("DMA'ing!\n");
}

void gb_keypress(struct gb* gb, enum key_e key) {
    gb->keysPressed &= ~key;
}

void gb_keyrelease(struct gb* gb, enum key_e key) {
    gb->keysPressed |= key;
}

void gb_schedule_dma(struct gb* gb, u8 highByte) {
    //printf("Scheduling DMA at %#04x\n", highByte << 8);
    gb->dmaScheduled = true;
    gb->dmaCycles = 160 * 4;
    gb->dmaAddress = highByte << 8;
    if(gb->dmaAddress > 0xDF00) {
        printf("\033[34mWarning: Attempting to run OAM DMA from %#04x, which is over 0xDF00. Results are unpredictable!\033[0m\n", gb->dmaAddress);
    }
}

// TODO: Remove this
u8* gb_get_mmap_ptr(struct gb* gb, u16 addr) {
    return gb->mmap + addr;
}

u8 gb_read8(struct gb* gb, u16 addr) {
    if(addr < 0x100 && gb->inBootrom) {
        return gb->bootrom[addr];
    }
    else if(addr < 0x8000) {
        return gb->cart.mbc->read8(&gb->cart, addr);
    }
    
    if(addr == 0xFF00) {
        u8 p1 = gb->mmap[addr];
        u8 selections = p1 & 0x30;
        // TODO: There must be a better way to do this
        // Oh yeah, and also fix it because it treats
        // DPAD buttons as action buttons
        if(selections == 0x30)
            return 0xF;
        else if(selections == 0x20) {
            return gb->keysPressed & 0xF;
        }
        else {
            return gb->keysPressed >> 4;
        }
    }
    return gb->mmap[addr];
}

void gb_write8(struct gb* gb, u16 addr, u8 byte) {
    if(addr < 0x8000) {
        return gb->cart.mbc->write8(&gb->cart, addr, byte);
    }

    gb->mmap[addr] = byte;

    if(addr == 0xFF46) {
        gb_schedule_dma(gb, byte);
    }
    if(addr == 0xFF50 && byte == 0x1) {
        gb_disable_bootrom(gb);
    }
}

u16 gb_read16(struct gb* gb, u16 addr) {
    if(addr < 0x100 && gb->inBootrom) {
        return (gb->bootrom[addr + 1] << 8) | gb->bootrom[addr];
    }
    else if(addr < 0x8000) {
        return gb->cart.mbc->read16(&gb->cart, addr);
    }

    return (gb->mmap[addr + 1] << 8) | gb->mmap[addr];
}

void gb_write16(struct gb* gb, u16 addr, u16 word) {
    if(addr < 0x8000) {
        return gb->cart.mbc->write16(&gb->cart, addr, word);
    }
    gb->mmap[addr] = word & 0xFF;
    gb->mmap[addr + 1] = word >> 8;
}
