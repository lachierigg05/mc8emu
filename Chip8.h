//
// Definitions and implementations of the Chip 8 opcodes
// Created by Lachie Rigg on 21/8/24.
//

#include <chrono>
#include <random>

#include "FontSet.h"

#ifndef MC8EMU_CHIP8_H
#define MC8EMU_CHIP8_H
class chip8 {
public:
    chip8();

    void LoadROM(const char *romName); // Load the rom instructions in the cpu emu
    void emulateCycle(); // Emulates a single cycle of the Chip 8 CPU

    [[nodiscard]] bool getDrawFlag() const;
    void setDrawFlag(bool flag);

    // Opcode methods
    void OP_00E0();
    void OP_00EE();
    void OP_1nnn();
    void OP_2nnn();
    void OP_3xkk();
    void OP_4xkk();
    void OP_5xy0();
    void OP_6xkk();
    void OP_7xkk();
    void OP_8xy0();
    void OP_8xy1();
    void OP_8xy2();
    void OP_8xy3();
    void OP_8xy4();
    void OP_8xy5();
    void OP_8xy6();
    void OP_8xy7();
    void OP_8xyE();
    void OP_9xy0();
    void OP_Annn();
    void OP_Bnnn();
    void OP_Cxkk();
    void OP_Dxyn();
    void OP_Ex9E();
    void OP_ExA1();
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();

private:
    uint16_t opcode;
    uint8_t memory[4096]; // 4KB memory in Chip 8 instruction set
    uint16_t V[16]; // Registers V0-VE
    uint16_t I; // Index register
    uint16_t pc; // Program counter
    uint32_t gfx[64 * 32]; // 2048 pixels contained in the graphics of Chip 8
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
    uint16_t sp;
    bool keypad[16];
    bool drawFlag;

    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

};
#endif //MC8EMU_CHIP8_H
