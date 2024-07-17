#include <stdio.h>
#include <stdlib.h>

#include <switch.h>

#include "gb.h"
#include "sdlctx.h"


struct gb gb;
struct sdlctx sdlctx;


int main(int argc, char** argv) {

    //consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);


    gb_init(&gb);

    FILE* bootrom;
    if((bootrom = fopen("/bootrom.bin", "rb")) == NULL) {
        printf("Error opening bootrom file!\n");
        // Destroy everything and abort
        gb_destroy(&gb);
        return 1;
    }
    fseek(bootrom, 0L, SEEK_END);
    long size = ftell(bootrom);
    rewind(bootrom);
    if(size != 256) {
        printf("Wrong size bootrom!\n");
        // Destroy everything and abort
        gb_destroy(&gb);
        return 1;
    }

    gb_readBootrom(&gb, bootrom);
    fclose(bootrom);

    FILE* rom;
    if((rom = fopen("/tetris.gb", "rb")) == NULL) {
        printf("Error opening rom file!\n");
        // Destroy everything and abort
        gb_destroy(&gb);
        return 1;
    }

    gb_readRom(&gb, rom);
    fclose(rom);

    cpu_set_logging_enabled(gb.cpu, true);
    // cpu_set_stop_at_bootrom(gb->cpu, true);

    // Create the sdl context
    if(sdlctx_init(&sdlctx) < 0) {
        gb_destroy(&gb);
        return 1;
    }
    
    bool sdlStopped = false;
    bool gbStopped = false;
    // Main loop
    while(appletMainLoop() && !sdlStopped && !gbStopped) {
        //consoleUpdate(NULL);

        bool frameCompleted = false;
        if(gb_run(&gb, &gbStopped, &frameCompleted) < 0) {
            break;
        }
        sdlctx_update(&sdlctx, &sdlStopped, frameCompleted, &gb);
        padUpdate(&pad);

        u64 kDown = padGetButtonsDown(&pad);
        if (kDown & HidNpadButton_Plus) break;
    }

    // Destroy the sdl context
    sdlctx_destroy(&sdlctx);

    // Destroy emulator instance
    gb_destroy(&gb);

    //consoleExit(NULL);

    return 0;
}
