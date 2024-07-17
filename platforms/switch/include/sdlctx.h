#pragma once
#include <SDL2/SDL.h>

#include "common.h"

struct gb;

enum WindowID {
    WINDOW_MAIN,

    WINDOW_COUNT
};

struct window_t
{
    int w;
    int h;
    SDL_Window* win;
    SDL_Surface* surface;
    u32* framebuffer;
    int id;
    int index;
};

struct sdlctx {
    struct window_t windows[WINDOW_COUNT];

    u64 lastUpdateTicks;
};

int sdlctx_init(struct sdlctx* ctx);
void sdlctx_update(struct sdlctx* ctx, bool* stopped, bool frameCompleted, struct gb* gb);
void sdlctx_destroy(struct sdlctx* ctx);