#include "ppu.h"
#include "gb.h"
#include "cpu.h"

#include <string.h>

const u32 gColors[4] = {
    0xFF9BBC0F, // Lightest
    0xFF8BAC0F,
    0xFF306230,
    0xFF0F380F  // Darkest
};

void ppu_init(struct ppu* ppu, struct gb* gb) {
    ppu->gb = gb;

    ppu->lcdc = (struct lcdc_t*) gb_get_mmap_ptr(gb, 0xFF40);
    ppu->stat = (struct stat_t*) gb_get_mmap_ptr(gb, 0xFF41);
    ppu->scy  = gb_get_mmap_ptr(gb, 0xFF42);
    ppu->scx  = gb_get_mmap_ptr(gb, 0xFF43);
    ppu->ly   = gb_get_mmap_ptr(gb, 0xFF44);
    ppu->lyc  = gb_get_mmap_ptr(gb, 0xFF45);
    ppu->bgp  = gb_get_mmap_ptr(gb, 0xFF47);
    ppu->obp0 = gb_get_mmap_ptr(gb, 0xFF48);
    ppu->obp1 = gb_get_mmap_ptr(gb, 0xFF49);
    ppu->wy   = gb_get_mmap_ptr(gb, 0xFF4A);
    ppu->wx   = gb_get_mmap_ptr(gb, 0xFF4B);
    ppu->objs = (struct obj_t*) gb_get_mmap_ptr(gb, 0xFE00);

    // Start in OAM Search
    ppu->stat->mode = 0x02;
    *ppu->scy = 0x00;
    *ppu->scx = 0x00;
    *ppu->ly = 0x00;
    *ppu->lyc = 0x00;
    *ppu->bgp = 0xE4;  // 0b11100100
    *ppu->obp0 = 0xE4; // ^
    *ppu->obp1 = 0xE4; // ^
    *ppu->wy = 0x00;
    *ppu->wx = 0x00;

    // Fill with gColors[0]
    for(int i = 0; i < 160*144; i++)
        ppu->framebuffer[i] = gColors[0];
}

bool ppu_run(struct ppu* ppu, int lastCpuCycles) {
    if(!ppu->lcdc->lcdcOn) {
        return false;
    }

    ppu->cyclesThisMode += lastCpuCycles;

    switch(ppu->stat->mode) {
        // hblank: 51 cycles
        case 0x00:
            if(ppu->cyclesThisMode >= 51) {
                ppu_hblank(ppu);
                // Incrememnt ly after hblank completes. If we hit line 144,
                // go to vblank, otherwise go to oam search
                (*ppu->ly)++;
                if(*ppu->ly >= 144) {
                    ppu->stat->mode = 0x01;
                    ppu->vblankCycles = ppu->cyclesThisMode % 51;
                    cpu_request_interrupt(ppu->gb->cpu, INTERRUPT_VBLANK);
                } else {
                    ppu->stat->mode = 0x02;
                }
                ppu->cyclesThisMode %= 51;
            }
            break;
        // vblank: 1140 cycles
        case 0x01:
            ppu->vblankCycles += lastCpuCycles;
            if(ppu->vblankCycles >= 114) {
                (*ppu->ly)++;
                ppu->vblankCycles %= 114;
            }
            if(ppu->cyclesThisMode >= 1140) {
                ppu_vblank(ppu);
                // TODO: Does LY still increase past 144 in vblank?
                *ppu->ly = 0;
                ppu->stat->mode = 0x02;
                ppu->cyclesThisMode %= 1140;

                return true;
            }
            break;
        // oam search: 20 cycles
        case 0x02:
            if(ppu->cyclesThisMode >= 20) {
                ppu_oamsearch(ppu);
                ppu->stat->mode = 0x03;
                ppu->cyclesThisMode %= 20;
            }
            break;
        // data transfer
        case 0x03:
            if(ppu->cyclesThisMode >= 43) {
                ppu_scanline(ppu);
                ppu->stat->mode = 0x00;
                ppu->cyclesThisMode %= 43;
            }
            break;
    }

    return false;
}

void ppu_destroy(struct ppu* ppu) {
    
}

void ppu_hblank(struct ppu* ppu) {

}

void ppu_vblank(struct ppu* ppu) {

}

void ppu_oamsearch(struct ppu* ppu) {
    u16 oamRAM = 0xFE00;
    u8 objH = (ppu->lcdc->obj8x16)? 16 : 8;
    u8 y = *ppu->ly;

    for(int a = 0; a < 40; a++) {
        int objY = (int) gb_read8(ppu->gb, oamRAM + (a * 4)) - objH;
        if(objY >= y && objY < (y + objH)) {
            ppu->scanlineObjs[ppu->objsThisScanline++] = a;
            if(ppu->objsThisScanline == 10) {
                break;
            }
        }
    }
}

void ppu_scanline(struct ppu* ppu)  {
    for(int x = 0; x < 20*8; x++) {
        // TODO: Implement SCX
        int y = *ppu->ly;
        u8 ySc = *ppu->scy;
        int xTile = x / 8;
        int yTile = (y / 8) + (ySc / 8);
        int xPixel = x % 8;
        int yPixel = y % 8;
        int yScPx = ySc % 8;
        yPixel += yScPx;
        
        u16 bgMapStart = (ppu->lcdc->bgMapArea)? 0x9C00 : 0x9800;
        if(yPixel > 7) {
            yPixel %= 8;
            yTile++;
        }
        yTile %= 32;
        u16 bgMapOffset = (yTile * 32) + xTile;
        if(bgMapOffset >= 0x400) {
            printf("Warning: BG Map offset %#04x is out of range!\n", bgMapOffset);
        }
        s8 tileId = (s8) gb_read8(ppu->gb, bgMapStart + bgMapOffset);
        u16 bgTilesOffset = (ppu->lcdc->bgTilesArea)? 0x8000 : 0x9000;
        u16 bgTileAddr = (ppu->lcdc->bgTilesArea)? (bgTilesOffset + ((u8)tileId * 16)) : (bgTilesOffset + (tileId * 16));

        u16 tileLineOffset = bgTileAddr + (yPixel * 2);
        
        u8 lineBytes[2] = { gb_read8(ppu->gb, tileLineOffset), gb_read8(ppu->gb, tileLineOffset + 1) };
        u8 pixelBitMask = (1 << (7 - xPixel));
        
        u8 colorHigh = (lineBytes[1] & pixelBitMask) >> (7 - xPixel);
        u8 colorLow =  (lineBytes[0] & pixelBitMask) >> (7 - xPixel);
        u8 twoBitColor = colorLow | (colorHigh << 1);
        u8 bgp = *ppu->bgp;
        u8 bgpColor = (bgp >> (twoBitColor * 2)) & 0x3;

        u32 finalColor = gColors[bgpColor];

        ppu->framebuffer[(y * 160) + x] = finalColor;
    }

    ppu_scanline_objs(ppu);
}

void ppu_scanline_objs(struct ppu* ppu) {
    u8 y = *ppu->ly;
    u16 oamRAM = 0xFE00;
    u16 tileRAM = 0x8000;
    u8 objH = (ppu->lcdc->obj8x16)? 16 : 8;

    for(int i = 0; i < ppu->objsThisScanline; i++) {
        u8 thisObj = ppu->scanlineObjs[i];
        u16 thisObjAddr = oamRAM + (thisObj * 4);
        u8 objY = gb_read8(ppu->gb, thisObjAddr);
        u8 objX = gb_read8(ppu->gb, thisObjAddr + 1);
        u8 objTile = gb_read8(ppu->gb, thisObjAddr + 2);
        u8 objAttr = gb_read8(ppu->gb, thisObjAddr + 3);
        // Get the line number of the tile we want
        u8 yPx = y - objY;
        yPx %= objH;
        if(yPx >= objH) {
            printf("OBJ pxY %d over sprite height!\n", yPx);
            continue;
        }
        // Don't draw tiles that are off the top of the screen
        if((objY + objH) <= 16) {
            printf("OBJ off top of screen! %d\n", objY);
            continue;
        }

        u8 lineByte1 = gb_read8(ppu->gb, tileRAM + (objTile * 16) + (yPx * 2));
        u8 lineByte2 = gb_read8(ppu->gb, tileRAM + (objTile * 16) + (yPx * 2) + 1);

        int xStart = objX - 8;
        int xPxStart = 0;
        if(xStart < 0) {
            xStart = 0;
            xPxStart = 8 - objX;
        }
        
        // printf("\nCorr: ID: %#02x, X: %#02x, Y: %#02x, Attr: %#02x\n", 
        //         gb_read8(ppu->gb, 0xFE00), gb_read8(ppu->gb, 0xFE01), gb_read8(ppu->gb, 0xFE02), gb_read8(ppu->gb, 0xFE03));
        // printf("Mine: ID: %#02x, X: %#02x, Y: %#02x, Attr: %#02x\n", 
        //         objY, objX, objTile, objAttr);
        // printf("xStart: %d, xPxStart: %d, y: %d, yPx: %d\n", xStart, xPxStart, y, yPx);
        for(int xPx = xPxStart; xPx < 8; xPx++) {
            u8 pixelBitMask = (1 << (7 - xPx));
        
            u8 colorHigh = (lineByte2 & pixelBitMask) >> (7 - xPx);
            u8 colorLow =  (lineByte1 & pixelBitMask) >> (7 - xPx);
            u8 twoBitColor = colorLow | (colorHigh << 1);
            u8 pal = (objAttr & 0x10)? *ppu->obp1 : *ppu->obp0;
            u8 bgpColor = (pal >> (twoBitColor * 2)) & 0x3;

            u32 finalColor = gColors[bgpColor];

            ppu->framebuffer[(y * 160) + xStart + xPx] = finalColor;
        }
    }

    ppu->objsThisScanline = 0;
}
