//
// Definitions and implementations of the Chip 8 opcodes
// Created by Lachie Rigg on 21/8/24.
//


#ifndef MC8EMU_CHIP8_H
#define MC8EMU_CHIP8_H
class chip8 {
public:
    static void initialise(); // Initialises the initial memory and register state
    void emulateCycle(); // Emulates a single cycle of the Chip 8 CPU

    [[nodiscard]] bool getDrawFlag() const;
    void setDrawFlag(bool flag);

private:
    bool drawFlag;
};
#endif //MC8EMU_CHIP8_H
