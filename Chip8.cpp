//
// Implementation of Chip 8 methods
// Created by Lachie Rigg on 21/8/24.
//

#include <sys/stat.h>
#include <fstream>

#include "Chip8.h"
#include "FontSet.h"

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;

chip8::chip8()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count()),
        pc(0x200),
        opcode(0),
        I(0),
        sp(0),
        delay_timer(0),
        sound_timer(0) {

    // Clear the display
    std::fill(std::begin(gfx), std::end(gfx), 0);

    // Clear the stack
    std::fill(std::begin(stack), std::end(stack), 0);

    // Clear the registers
    std::fill(std::begin(V), std::end(V), 0);

    // Initialise the memory
    std::fill(std::begin(memory), std::end(memory), 0);

    // Load font set into memory
    std::copy(std::begin(fontset), std::end(fontset), memory + FONTSET_START_ADDRESS);

    // Initialise the random number generator
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void chip8::LoadROM(const char *romName) {
    std::ifstream rom;
    rom.open(romName, std::ios::binary | std::ios::ate);
    if (!rom.is_open()) {
        throw std::runtime_error("Could not read from the file \'%s\', please try a different chip 8 rom.");
    }

    // Extract the size of the file by getting the position
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

// Opcode Implementation

// OP_00E0 - Clears the display
void chip8::OP_00E0() {
    std::fill(std::begin(gfx), std::end(gfx), 0);
}

// OP_00EE - Returns from a subroutine
void chip8::OP_00EE() {
    sp--;
    pc = stack[sp];
}

// OP_1nnn - Jump to location nnn
void chip8::OP_1nnn() {
    uint16_t address = opcode & 0x0FFFu; // Extract the address of nnn from opcode
    pc = address;
}

// OP_2nnn - Call subroutine at nnn
void chip8::OP_2nnn() {
    uint16_t address = opcode & 0x0FFFu;
    stack[sp] = pc;
    sp++;
    pc = address;
}

// OP3xkk - Skip the next instruction if V[x] = kk
void chip8::OP_3xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (V[Vx] == byte) {
        pc += 2;
    }
}

// OP4xkk - Skip the next instruction if V[x] != kk
void chip8::OP_4xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (V[Vx] != byte) {
        pc += 2;
    }
}

// OP5xy0 - Skip the next instruction if V[x] = V[y]
void chip8::OP_5xy0() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    if (V[x] == V[y]) {
        pc += 2;
    }
}

// OP6xkk - Set V[x] = kk
void chip8::OP_6xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[Vx] = byte;
}

// OP7xkk - Set V[x] = V[x] + kk
void chip8::OP_7xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[Vx] += byte;
}

// OP8xy0 - Set V[x] = V[y]
void chip8::OP_8xy0() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    V[x] = V[y];
}

// OP8xy1 - Set V[x] = V[x] | V[y]
void chip8::OP_8xy1() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    V[x] |= V[y];
}

// OP8xy2 - Set V[x] = V[x] & V[y]
void chip8::OP_8xy2() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    V[x] &= V[y];
}

// OP8xy3 - Set V[x] = V[x] ^ V[y]
void chip8::OP_8xy3() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    V[x] ^= V[y];
}

// OP8xy4 - Set V[x] = V[x] + V[y], set VF = carry
void chip8::OP_8xy4() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = V[x] + V[y];

    if (sum > 255U) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[x] = sum & 0xFFu;
}