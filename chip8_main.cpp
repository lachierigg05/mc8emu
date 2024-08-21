#include <iostream>

#include "CPUStructures.h"
#include "Chip8.h"

chip8 cpu_emu;

int main(int argc, char **argv) {
    // Setup of graphics rendering and register input callbacks
    setupGraphics();
    setupInput();

    // Initialise the chip8 emulation and load in game data to memory
    cpu_emu.initialise();
    cpu_emu.loadGame("");

    while (true) {
        cpu_emu.emulateCycle();
        if (cpu_emu.drawFlag) {
            drawGraphics();
            cpu_emu.setKeys();
        }
    }

    return 0;
}
