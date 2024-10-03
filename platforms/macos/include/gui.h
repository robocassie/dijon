#pragma once
#include "common.h"
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;

struct gb;
struct gui {
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_Texture *gameTex;

    u64 lastUpdateTicks;
};

int gui_init(struct gui* gui);
void gui_destroy(struct gui* gui);
void gui_update(struct gui* gui, struct gb* gb, bool* stopped, bool frameCompleted);
