#include <iostream>
#include <fstream>

#ifndef CHIP8_H
#define CHIP8_H

const uint FONTSTART {0x050};
const uint FONTEND {0x0A0};
const uint ROMSTART {0x200};
const uint FLAGREGISTER {15};


class chip8 {
public:
    // member vars
    uint16_t opcode{};
    uint8_t keyboard[16]{};
    uint8_t registers[16] {};
    uint8_t memory[4096] {};
    uint16_t indexRego {};
    uint16_t pc {};
    uint16_t stack[16] {};
    uint8_t sp {};
    uint8_t delayTimer {};
    uint8_t soundTimer {};
    uint32_t display[64 * 32] {};
    // member functions
    void loadRom(const char* romFile);
    int giveRandInt();
    // constructors
    chip8();
    // Opcodes
    void Ox00E0();
    void Ox1NNN();
    void Ox2NNN();
    void Ox3XKK();
    void Ox4XKK();
    void Ox5XY0();
    void Ox6XKK();
    void Ox7XKK();
    void Ox8XY0();
    void Ox8XY1();
    void Ox8XY2();
    void Ox8XY3();
    void Ox8XY4();
    void Ox8XY5();
    void Ox8XY6();
    void Ox8XY7();
    void Ox8XYE();
    void Ox9XY0();
    void OxANNN();
    void OxBNNN();
    void OxCXKK();
    void OxDXYN();
    void OxEX9E();
    void OxEXA1();
    void OxFX07();
    void OxFX0A();
    void OxFX15();
    void OxFX18();
    void OxFX1E();
    void OxFX29();
    void OxFX33();
    void OxFX55();
    void OxFX65();
}


#endif