#include <iostream>

#include "Chip8.h"

chip8 cpu_emu;

int main(int argc, char **argv) {
    try {
        // Get the ROM name and load in the instructions
        const char *filename = argv[1];
        cpu_emu.LoadROM(filename);
    } catch(const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    };

    return 0;
}
