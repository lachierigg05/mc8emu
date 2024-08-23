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
const auto SPRITE_WIDTH = 8;

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

// OP8xy5 - Set V[x] = V[x] - V[y], set VF = NOT borrow
void chip8::OP_8xy5() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    if (V[x] > V[y]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[x] -= V[y];
}

// OP8xy6 - Set V[x] = V[x] >> 1, set VF = carry
void chip8::OP_8xy6() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;

    V[0xF] = V[x] & 0x1u;
    V[x] >>= 1;
}

// OP8xy7 - Set V[x] = V[y] - V[x], set VF = NOT borrow
void chip8::OP_8xy7() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    if (V[y] > V[x]) {
        V[0xF] = 1;
    } else {
        V[0xF] = 0;
    }

    V[x] = V[y] - V[x];
}

// OP8xyE - Set V[x] = V[x] << 1, set VF = carry
void chip8::OP_8xyE() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;

    V[0xF] = V[x] >> 7u;
    V[x] <<= 1;
}

// OP9xy0 - Skip the next instruction if V[x] != V[y]
void chip8::OP_9xy0() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t y = (opcode & 0x00F0u) >> 4u;

    if (V[x] != V[y]) {
        pc += 2;
    }
}

// OPAnnn - Set I = nnn
void chip8::OP_Annn() {
    uint16_t address = opcode & 0x0FFFu;
    I = address;
}

// OPBnnn - Jump to location nnn + V[0]
void chip8::OP_Bnnn() {
    uint16_t address = opcode & 0x0FFFu;
    pc = address + V[0];
}

// OPCxkk - Set V[x] = random byte AND kk
void chip8::OP_Cxkk() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    V[x] = randByte(randGen) & byte;
}

// OPDxyn - Display n-byte sprite starting at memory location I at (V[x], V[y]), set VF = collision
void chip8::OP_Dxyn() {
    // Get the x and y coordinates
    uint8_t x = V[(opcode & 0x0F00u) >> 8u];
    uint8_t y = V[(opcode & 0x00F0u) >> 4u];
    uint8_t height = opcode & 0x000Fu;

    // Set VF to 0
    V[0xF] = 0;

    for (unsigned int row = 0; row < height; row++) {
        // Get the sprite byte
        uint8_t spriteByte = memory[I + row];

        for (unsigned int col = 0; col < SPRITE_WIDTH; col++) {
            uint8_t spritePixel = spriteByte & (0x80u >> col);
            uint32_t *screenPixel = &gfx[(y + row) * 64 + (x + col)];

            // Collision detection
            if (spritePixel) {
                // If the screen pixel is set to 1, set VF to 1
                if (*screenPixel == 0xFFFFFFFF) {
                    V[0xF] = 1;
                }
                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

// OPEx9E - Skip the next instruction if the key with the value of V[x] is pressed
void chip8::OP_Ex9E() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t key = V[x];

    if (keypad[key]) {
        pc += 2;
    }
}

// OPExA1 - Skip the next instruction if the key with the value of V[x] is not pressed
void chip8::OP_ExA1() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t key = V[x];

    if (!keypad[key]) {
        pc += 2;
    }
}

// OPFx07 - Set V[x] = delay timer value
void chip8::OP_Fx07() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    V[x] = delay_timer;
}

// OPFx0A - Wait for a key press, store the value of the key in V[x]
void chip8::OP_Fx0A() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;

    if (keypad[0]) {
        V[x] = 0;
    } else if (keypad[1]) {
        V[x] = 1;
    } else if (keypad[2]) {
        V[x] = 2;
    } else if (keypad[3]) {
        V[x] = 3;
    } else if (keypad[4]) {
        V[x] = 4;
    } else if (keypad[5]) {
        V[x] = 5;
    } else if (keypad[6]) {
        V[x] = 6;
    } else if (keypad[7]) {
        V[x] = 7;
    } else if (keypad[8]) {
        V[x] = 8;
    } else if (keypad[9]) {
        V[x] = 9;
    } else if (keypad[10]) {
        V[x] = 10;
    } else if (keypad[11]) {
        V[x] = 11;
    } else if (keypad[12]) {
        V[x] = 12;
    } else if (keypad[13]) {
        V[x] = 13;
    } else if (keypad[14]) {
        V[x] = 14;
    } else if (keypad[15]) {
        V[x] = 15;
    } else {
        pc -= 2;
    }
}

// OPFx15 - Set delay timer = V[x]
void chip8::OP_Fx15() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    delay_timer = V[x];
}

// OPFx18 - Set sound timer = V[x]
void chip8::OP_Fx18() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    sound_timer = V[x];
}

// OPFx1E - Set I = I + V[x]
void chip8::OP_Fx1E() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    I += V[x];
}

// OPFx29 - Set I = location of sprite for digit V[x]
void chip8::OP_Fx29() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = V[x];

    I = FONTSET_START_ADDRESS + (5 * digit);
}

// OPFx33 - Store BCD representation of V[x] in memory locations I, I+1, I+2
void chip8::OP_Fx33() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;
    uint8_t value = V[x];

    // Ones-place
    memory[I + 2] = value % 10;
    value /= 10;

    // Tens-place
    memory[I + 1] = value % 10;
    value /= 10;

    // Hundreds-place
    memory[I] = value % 10;
}

// OPFx55 - Store V[0] to V[x] in memory starting at location I
void chip8::OP_Fx55() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= x; i++) {
        memory[I + i] = V[i];
    }
}

// OPFx65 - Read V[0] to V[x] from memory starting at location I
void chip8::OP_Fx65() {
    uint8_t x = (opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= x; i++) {
        V[i] = memory[I + i];
    }
}