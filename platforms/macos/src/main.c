#include <stdio.h>
#include <stdlib.h>

#include "gb.h"
#include "cpu.h"
#include "gui.h"


int main(int argc, char** argv) {

    struct gb gb;
    struct gui gui;

    if(argc < 2) {
        printf("You must include a bootrom PATH in the arguments!\n");
        return 1;
    }

    gb_init(&gb);

    FILE* bootrom;
    if((bootrom = fopen(argv[1], "rb")) == NULL) {
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
    if((rom = fopen(argv[2], "rb")) == NULL) {
        printf("Error opening rom file!\n");
        // Destroy everything and abort
        gb_destroy(&gb);
        return 1;
    }

    gb_readRom(&gb, rom);
    fclose(rom);

    if(argc > 3) {
        for(int i = 3; i < argc; i++) {
            if(argv[i][0] == '-') {
                switch(argv[i][1]) {
                    case 'v':
                        cpu_set_logging_enabled(gb.cpu, true);
                        break;
                    case 'b':
                        cpu_set_stop_at_bootrom(gb.cpu, true);
                        break;
                }
            }
        }
    }

    // Create the gui
    if(gui_init(&gui) < 0) {
        gb_destroy(&gb);
        return 1;
    }
    
    bool sdlStopped = false;
    bool gbStopped = false;
    // Main loop
    while(!sdlStopped && !gbStopped) {
        bool frameCompleted = false;
        if(gb_run(&gb, &gbStopped, &frameCompleted) < 0) {
            break;
        }
        gui_update(&gui, &gb, &sdlStopped, frameCompleted);
    }

    // Destroy the gui
    gui_destroy(&gui);
    // Destroy emulator instance
    gb_destroy(&gb);

    return 0;
}
