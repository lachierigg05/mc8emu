//
// Implementation of Chip 8 methods
// Created by Lachie Rigg on 21/8/24.
//

#include "Chip8.h"
#include "CPUStructures.h"

void chip8::initialise() {
    // Initialise memory and registers
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    // Clear the display
    for (uint8_t &i : gfx) {
        i = 0;
    }

    // Clear the stack
    for (uint16_t &i : stack) {
        i = 0;
    }

    // Clear the registers
    for (uint16_t &r : V) {
        r = 0;
    }
}

void chip8::emulateCycle() {
    // Fetch opcode
    opcode = memory[cp] << 8 | memory[cp + 1];
    // Decode opcode

    // Execute opcode



    // Update timers
}

bool chip8::getDrawFlag() const {
    return drawFlag;
}

void chip8::setDrawFlag(bool flag) {
    drawFlag = flag;
}
