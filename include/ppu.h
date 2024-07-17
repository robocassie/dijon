#pragma once

#include "common.h"

extern const u32 gColors[4];

struct gb;

struct obj_t {
    u8 y;
    u8 x;
    u8 chr;
    struct {
        u8 cgbPal   : 3;
        u8 cgbBank  : 1;
        u8 dmgPal   : 1;
        u8 xFlip    : 1;
        u8 yFlip    : 1;
        u8 priority : 1;
    } attr;
};

struct ppu {
    struct gb* gb;
    u32 framebuffer[160*144];
    int cyclesThisMode;
    int vblankCycles;

    u8 objsThisScanline;
    u8 scanlineObjs[10];

    u8* ly;
    u8* lyc;
    u8* scy;
    u8* scx;
    u8* wy;
    u8* wx;

    struct lcdc_t {
        u8 bgDispOn    : 1; // In CGB mode, BG display is always on
        u8 objDispOn   : 1;
        u8 obj8x16     : 1;
        u8 bgMapArea   : 1; // 0: 9800h-9BFFh; 1: 9C00h-9FFFh
        u8 bgTilesArea  : 1; // 0: 8800h-97FFh; 1: 8000h-8FFFh
        u8 windowOn    : 1;
        u8 winMapArea  : 1; // 0: 9800h-9BFFh; 1: 9C00h-9FFFh
        u8 lcdcOn      : 1;
    } *lcdc;
    struct stat_t {
        u8 mode     : 2;
        u8 lycMatch : 1;
        struct {
            // TODO: Wtf are these?
            u8 mode00sel : 1;
            u8 mode01sel : 1;
            u8 mode10sel : 1;
            u8 lycMatch  : 1;
        } interruptSel;
        u8 filler   : 1;
    } *stat;

    u8* bgp;
    u8* obp0;
    u8* obp1;

    struct obj_t* objs;
};

void ppu_init(struct ppu*, struct gb* gb);
bool ppu_run(struct ppu*, int lastCpuCycles);
void ppu_destroy(struct ppu*);

void ppu_hblank(struct ppu*);
void ppu_vblank(struct ppu*);
void ppu_oamsearch(struct ppu*);
void ppu_scanline(struct ppu*);
void ppu_scanline_objs(struct ppu*);
