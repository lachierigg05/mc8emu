//
// Created by Lachie Rigg on 21/8/24.
//

#ifndef MC8EMU_CPUSTRUCTURES_H
#define MC8EMU_CPUSTRUCTURES_H

#include <cstdint>

uint16_t opcode;
uint8_t memory[4096]; // 4KB memory in Chip 8 instruction set
uint16_t V[16]; // Registers V0-VE
uint16_t I; // Index register
uint16_t pc; // Program counter
uint8_t gfx[64 * 32]; // 2048 pixels contained in the graphics of Chip 8
uint8_t delay_timer;
uint8_t sound_timer;
uint16_t stack;
uint16_t sp;
uint8_t key; // Holds the state of the keypad

#endif //MC8EMU_CPUSTRUCTURES_H
