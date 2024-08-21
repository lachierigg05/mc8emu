//
// Implementation of Chip 8 methods
// Created by Lachie Rigg on 21/8/24.
//

#include "Chip8.h"

void chip8::initialise() {
    // Initialise memory and registers
}

void chip8::emulateCycle() {
    // Fetch opcode
    opcode = memory[cp] << 8 | memory[cp + 1];
    // Decode opcode

    // Execute opcode



    // Update timers
}
