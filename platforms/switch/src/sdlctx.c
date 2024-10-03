#include "sdlctx.h"
#include "gb.h"
#include "ppu.h"

static int gMainWindowW = 1920;
static int gMainWindowH = 1080;

static int gBGMapWindowW = 256;
static int gBGMapWindowH = 256;
static int gBGMapWindowBuffer = 5;


int sdlctx_initWindow(struct sdlctx* ctx, enum WindowID id, const char* title, int x, int y, int w, int h, u32 flags) {
    
    ctx->windows[id].win = SDL_CreateWindow(title, x, y, w, h, flags);

    if(ctx->windows[id].win == NULL) {
        printf("SDL Error creating window id %d: %s\n", id, SDL_GetError());
        return -1;
    }
    else {
        ctx->windows[id].index = id;
        ctx->windows[id].id = SDL_GetWindowID(ctx->windows[id].win);
        ctx->windows[id].surface = SDL_GetWindowSurface(ctx->windows[id].win);
        ctx->windows[id].framebuffer = (u32*) ctx->windows[id].surface->pixels;
        SDL_FillRect(ctx->windows[id].surface, NULL, SDL_MapRGBA(ctx->windows[id].surface->format, 0x00, 0x00, 0x00, 0xFF));
        SDL_UpdateWindowSurface(ctx->windows[id].win);
    }
    return 0;
}

int sdlctx_init(struct sdlctx* ctx) {

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        printf("SDL error: %s\n", SDL_GetError());
        return -1;
    } else {
        u32 mainWindowFlags = 0;//SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP;
        if(sdlctx_initWindow(ctx, WINDOW_MAIN, "dijon", 0, 0, gMainWindowW, gMainWindowH, mainWindowFlags) < 0) {
            return -1;
        }

        // int mainWindowX, mainWindowY;
        // SDL_GetWindowPosition(ctx->windows[WINDOW_MAIN].win, &mainWindowX, &mainWindowY);
        // // 256x256 with a 5 pixel border on all sides
        // int border = gBGMapWindowBuffer * 2;
        // if(sdlctx_initWindow(ctx, WINDOW_BGMAP, "BG Map", mainWindowX + gMainWindowW + 10, mainWindowY, gBGMapWindowW + border, gBGMapWindowH + border, SDL_WINDOW_SHOWN) < 0) {
        //     return -1;
        // }

        //SDL_RaiseWindow(ctx->windows[WINDOW_MAIN].win);
    }

    //ctx->lastUpdateTicks = SDL_GetTicks64();

    return 0;
}

struct window_t* sdlctx_get_window_from_internal_id(struct sdlctx* ctx, u32 internalID) {
    for(int i = 0; i < WINDOW_COUNT; i++) {
        if(ctx->windows[i].id == internalID) {
            return &ctx->windows[i];
        }
    }
    return NULL;
}

void sdlctx_renderMainWindow(struct sdlctx* ctx, u32* ppuFramebuffer) {
    SDL_LockSurface(ctx->windows[WINDOW_MAIN].surface);
    //memcpy(ctx->windows[WINDOW_MAIN].framebuffer, ppuFramebuffer, (gMainWindowW * gMainWindowH * sizeof(u32)));
    for(int y = 0; y < 144; y++) {
        for(int x = 0; x < 160; x++) {
            ctx->windows[WINDOW_MAIN].framebuffer[(y * gMainWindowW) + x] = 0xFFFF00FF;// ppuFramebuffer[(y * 160) + x];
        }
    }
    SDL_UnlockSurface(ctx->windows[WINDOW_MAIN].surface);

    SDL_UpdateWindowSurface(ctx->windows[WINDOW_MAIN].win);
}

void sdlctx_renderBGMapWindow(struct sdlctx* ctx, struct gb* gb) {
    
    SDL_LockSurface(ctx->windows[WINDOW_BGMAP].surface);

    u16 bgMapStart = (gb->ppu->lcdc->bgMapArea == 0)? 0x9800 : 0x9C00;
    u16 bgTilesStart = (gb->ppu->lcdc->bgTilesArea == 0)? 0x8800 : 0x8000;

    for(int y = 0; y < gBGMapWindowH; y++) {
        for(int x = 0; x < gBGMapWindowW; x++) {
            u8 scy = *gb->ppu->scy;
            u8 scx = *gb->ppu->scx;
            int bgMapX = x / 8;
            int bgMapY = y / 8;
            int xPixel = x % 8;
            int yPixel = y % 8;
            u16 bgMapOffset = (bgMapY * 32) + bgMapX;
            u8 tileId = gb_read8(gb, bgMapStart + bgMapOffset);
            u16 bgTilesOffset = tileId * 16;
            u16 tileLineOffset = bgTilesStart + bgTilesOffset + (yPixel * 2);
            u8 tileLineBytes[2] = { gb_read8(gb, tileLineOffset), gb_read8(gb, tileLineOffset + 1) };
            u8 pixelBitMask = (1 << (7 - xPixel));
            u8 colorHigh = (tileLineBytes[1] & pixelBitMask) >> (7 - xPixel);
            u8 colorLow =  (tileLineBytes[0] & pixelBitMask) >> (7 - xPixel);
            u8 internalColor = (colorHigh << 1) | colorLow;
            u8 bgPalColor = (*gb->ppu->bgp >> (internalColor * 2)) & 0x3;
            u32 finalColor = gColors[bgPalColor];

            // 5 pixel border on all sides
            int totalWidth = gBGMapWindowW + (gBGMapWindowBuffer * 2);
            ctx->windows[WINDOW_BGMAP].framebuffer[((y + 5) * totalWidth) + x + 5] = finalColor;

            // Draw the red viewport
            bool yWrapsAround = ((int)(scy) + 144) > 256;
            bool xWrapsAround = ((int)(scx) + 160) > 256;

            if(( (y == scy || y == (u8)(scy + 144)) && ((x >= scx && x < scx + 160) || (xWrapsAround && x < (u8)(scx + 160))) ) ||
               ( (x == scx || x == (u8)(scx + 160)) && ((y >= scy && y < scy + 144) || (yWrapsAround && y < (u8)(scy + 144))) )
            ) {
                ctx->windows[WINDOW_BGMAP].framebuffer[((y + 5) * 266) + x + 5] = 0xFFFF0000;
            }
        }
    }

    SDL_UnlockSurface(ctx->windows[WINDOW_BGMAP].surface);
    SDL_UpdateWindowSurface(ctx->windows[WINDOW_BGMAP].win);
}

void sdlctx_update(struct sdlctx* ctx, bool* stopped, bool completedFrame, struct gb* gb) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch(e.type) {
            case SDL_JOYBUTTONDOWN:
                if(e.jbutton.which == 0) {
                    if (e.jbutton.button == 10) {
                        // (+) button down
                        *stopped = true;
                    }
                }
                break;
        }
    }

    // Every 1/60th of a second, we copy the internal framebuffer from
    // the PPU into the external framebuffer in the sdl context and refresh
    // TODO: There are probably 30 better ways to do this
    // u64 newUpdateTicks = SDL_GetTicks64();
    // if(newUpdateTicks - ctx->lastUpdateTicks >= (1000 / 60.0F)) {
    if(completedFrame) {
        sdlctx_renderMainWindow(ctx, gb->ppu->framebuffer);
        // sdlctx_renderBGMapWindow(ctx, gb);

        // ctx->lastUpdateTicks = newUpdateTicks;
    }
}

void sdlctx_destroy(struct sdlctx* ctx) {
    for(int i = 0; i < WINDOW_COUNT; i++) {
        SDL_DestroyWindow(ctx->windows[i].win);
    }
    SDL_Quit();
}
