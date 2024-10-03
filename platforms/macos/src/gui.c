#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#define CIMGUI_USE_SDL2
#define CIMGUI_USE_SDLRENDERER2
#include "cimgui.h"
#include "cimgui_impl.h"
#include <SDL2/SDL.h>

#include "gui.h"
#include "gb.h"
#include "cpu.h"
#include "ppu.h"


int gui_init(struct gui* gui) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL error: %s\n", SDL_GetError());
        return -1;
    }

    gui->win = SDL_CreateWindow("dijon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    gui->ren = SDL_CreateRenderer(gui->win, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    gui->gameTex = SDL_CreateTexture(gui->ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 160, 144);


    igCreateContext(NULL);
    ImGuiIO *io = igGetIO();

    ImGuiStyle *style = igGetStyle();
    igStyleColorsDark(style);

    ImGui_ImplSDL2_InitForSDLRenderer(gui->win, gui->ren);
    ImGui_ImplSDLRenderer2_Init(gui->ren);

    gui->lastUpdateTicks = SDL_GetTicks64();

    return 0;
}

void gui_destroy(struct gui* gui) {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    igDestroyContext(NULL);

    SDL_DestroyRenderer(gui->ren);
    SDL_DestroyWindow(gui->win);
    SDL_Quit();
}

void gui_render(struct gui* gui, struct gb* gb, bool frameCompleted) {

    // Every 1/60th of a second, refresh the gui
    u64 newUpdateTicks = SDL_GetTicks64();
    if(newUpdateTicks - gui->lastUpdateTicks >= (1000 / 60.0F)) {

        gui->lastUpdateTicks = newUpdateTicks;

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        igNewFrame();

        if(igBeginMainMenuBar()) {
            if(igBeginMenu("File", true)) {
                igMenuItem_Bool("Open ROM", "O", false, true);
                igEndMenu();
            }
            igEndMainMenuBar();
        }

        igBegin("Main view", NULL, ImGuiWindowFlags_AlwaysAutoResize);
            u32 *textureBuffer;
            int pitch;
            SDL_LockTexture(gui->gameTex, NULL, (void**)&textureBuffer, &pitch);
            memcpy(textureBuffer, gb->ppu->framebuffer, 160 * 144 * sizeof(u32));
            SDL_UnlockTexture(gui->gameTex);
        igImage((ImTextureID) gui->gameTex,
                (ImVec2){160, 144},
                (ImVec2){0.0f, 0.0f},
                (ImVec2){1.0f, 1.0f},
                (ImVec4){1.0f, 1.0f, 1.0f, 1.0f},
                (ImVec4){1.0f, 1.0f, 1.0f, 0.0f});
        igEnd();

        igRender();
        SDL_SetRenderDrawColor(gui->ren, 0x73, 0x8C, 0x99, 0xFF);
        SDL_RenderClear(gui->ren);

        ImGui_ImplSDLRenderer2_RenderDrawData(igGetDrawData(), gui->ren);
        SDL_RenderPresent(gui->ren);
    }
}

void gui_update(struct gui* gui, struct gb* gb, bool* stopped, bool frameCompleted) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        ImGui_ImplSDL2_ProcessEvent(&e);

        if(e.type == SDL_QUIT) {
            *stopped = true;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.sym) {
                case SDLK_SPACE: gb->cpu->paused = !gb->cpu->paused; break;
                case SDLK_w:      gb_keypress(gb, GB_KEY_UP);     break;
                case SDLK_a:      gb_keypress(gb, GB_KEY_LEFT);   break;
                case SDLK_s:      gb_keypress(gb, GB_KEY_DOWN);   break;
                case SDLK_d:      gb_keypress(gb, GB_KEY_RIGHT);  break;
                case SDLK_o:      gb_keypress(gb, GB_KEY_A);      break;
                case SDLK_p:      gb_keypress(gb, GB_KEY_B);      break;
                case SDLK_RETURN: gb_keypress(gb, GB_KEY_START);  break;
                case SDLK_QUOTE:  gb_keypress(gb, GB_KEY_SELECT); break;
            }
        }
        else if (e.type == SDL_KEYUP) {
            switch(e.key.keysym.sym) {
                case SDLK_w:      gb_keyrelease(gb, GB_KEY_UP);     break;
                case SDLK_a:      gb_keyrelease(gb, GB_KEY_LEFT);   break;
                case SDLK_s:      gb_keyrelease(gb, GB_KEY_DOWN);   break;
                case SDLK_d:      gb_keyrelease(gb, GB_KEY_RIGHT);  break;
                case SDLK_o:      gb_keyrelease(gb, GB_KEY_A);      break;
                case SDLK_p:      gb_keyrelease(gb, GB_KEY_B);      break;
                case SDLK_RETURN: gb_keyrelease(gb, GB_KEY_START);  break;
                case SDLK_QUOTE:  gb_keyrelease(gb, GB_KEY_SELECT); break;
            }
        }
    }

    gui_render(gui, gb, frameCompleted);
}
