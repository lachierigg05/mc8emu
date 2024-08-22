//
// Implementation of Chip 8 methods
// Created by Lachie Rigg on 21/8/24.
//

#include <sys/stat.h>
#include <fstream>

#include "Chip8.h"

const unsigned int START_ADDRESS = 0x200;

chip8::chip8() {
    // Initialise relevant values
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;

    // Clear the display
    std::fill(std::begin(gfx), std::end(gfx), 0);

    // Clear the stack
    std::fill(std::begin(stack), std::end(stack), 0);

    // Clear the registers
    std::fill(std::begin(V), std::end(V), 0);

    // Initialise the memory
    std::fill(std::begin(memory), std::end(memory), 0);
}

void chip8::LoadROM(const char *romName) {
    std::ifstream rom;
    rom.open(romName, std::ios::binary | std::ios::ate);
    if (!rom.is_open()) {
        throw std::runtime_error("Could not read from the file \'%s\', please try a different chip 8 rom.");
    }

    auto const size = rom.tellg();
    rom.seekg(0, std::ios::beg);

    // Read file into the buffer
    char *buffer = new char[size];
    rom.read(buffer, size);
    if (!rom) {
        if (!rom.eof()) {
            throw std::runtime_error("Error reading from the ROM.");
        }
    }

    // Load in the instructions to the chip 8 memory
    for (auto i = 0; i < size; i++) {
        memory[START_ADDRESS + i] = buffer[i];
    }
}

void chip8::emulateCycle() {
}

bool chip8::getDrawFlag() const {
    return drawFlag;
}

void chip8::setDrawFlag(bool flag) {
    drawFlag = flag;
}
