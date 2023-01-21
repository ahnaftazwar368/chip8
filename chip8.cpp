#include <iostream>
#include <fstream>
#include <algorithm>
#include "chip8.h"

// Initialise the entire chip8 system by loading the first ROM instruction
// stored at 0x200
chip8::chip8() : pc(ROMSTART), sp(0) {
    uint8_t fontset[80] ={
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for (int i = 0; i < 80; i++) {
        memory[FONTSTART + i] = fontset[i];
    }
}

// Load the ROM file into the memory for use 
void chip8::loadRom(const char* romFile) {
    std::ifstream file(romFile, std::ios::binary);

    if (file.is_open()) {
        // find the size of the current file and create a buffer for the value
        file.seekg(0, file.end);
        int fileSize = file.tellg();
        char *buffer = new char[fileSize];

        // rewind the file and then copy all contents to buffer
        file.seekg(0, file.beg);
        file.read(buffer, fileSize);
        file.close();

        // load buffer to memory (starting from 0x200)
        for (int i = 0; i < fileSize; i++) {
            chip8::memory[ROMSTART + i] = buffer[i];
        }
    }

    delete[] buffer;
}

// Random num generator
int chip8::giveRandInt() {
    std::srand((unsigned) time(NULL));
    return rand() % 256;
}

// All Opcodes
void chip8::Ox00E0() {
    std::fill(display, display + (64*32), 0);
}

void chip8::Ox00EE() {
    sp--;
    pc = stack[sp];
}

void chip8::Ox1NNN() {
    // Opcode is made of 16 bits, 4 of which are used to give instruction of
    // which exact instruction is being called while the following 12 bits
    // give any operands that are contained in the opcode. Hence the bitwise
    // below gets rid of the first 4 bytes and captures the target address.
    uint16_t address = opcode & 0x0FFF;
    pc = address;
}

void chip8::Ox2NNN() {
    uint16_t address = opcode & 0x0FFF;
    stack[sp] = pc;
    sp++;
    pc = address;
}

void chip8::Ox3XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = opcode & 0x00FF;

    if (registers[targetRego] == targetValue) {
        pc += 2;
    }
}

void chip8::Ox4XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = opcode & 0x00FF;

    if (registers[targetRego] != targetValue) {
        pc += 2;
    }
}

void chip8::Ox5XY0() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    if (registers[targetRegoX] == registers[targetRegoY]) {
        pc += 2;
    }
}

void chip8::Ox6XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = (opcode & 0x00FF);

    registers[targetRego] = targetValue;
}

void chip8::Ox7XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = (opcode & 0x00FF);

    registers[targetRego] += targetValue;
}

void chip8::Ox8XY0() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoY];
}

void chip8::Ox8XY1() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoX] | registers[targetRegoY];
}

void chip8::Ox8XY2() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoX] & registers[targetRegoY];
}

void chip8::Ox8XY3() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoX] ^ registers[targetRegoY];
}

void chip8::Ox8XY4() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    int sum = registers[targetRegoX] + registers[targetRegoY];

    registers[FLAGREGISTER] = sum > 255 ? 1 : 0;

    registers[targetRegoX] = sum & 0x00FF;
}

void chip8::Ox8XY5() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    int diff = registers[targetRegoX] - registers[targetRegoY];

    registers[FLAGREGISTER] = diff > 0 ? 1 : 0;

    registers[targetRegoX] = diff;
}

void chip8::Ox8XY6() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;

    if (registers[targetRego] & 0x0001) {
        registers[FLAGREGISTER] = 1;
    } else {
        registers[FLAGREGISTER] = 0;
        registers[targetRego] /= 2;
    }
}

void chip8::Ox8XY7() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    int diff = registers[targetRegoX] - registers[targetRegoY];

    registers[FLAGREGISTER] = diff < 0 ? 1 : 0;

    registers[targetRegoX] = registers[targetRegoY] - registers[targetRegoX];
}

void chip8::Ox8XYE() {
    
}
void chip8::Ox9XY0() {
    
}
void chip8::OxANNN() {
    
}
void chip8::OxBNNN() {
    
}
void chip8::OxCXKK() {
    
}
void chip8::OxDXYN() {
    
}
void chip8::OxEX9E() {
    
}
void chip8::OxEXA1() {
    
}
void chip8::OxFX07() {
    
}
void chip8::OxFX0A() {
    
}
void chip8::OxFX15() {
    
}
void chip8::OxFX18() {
    
}
void chip8::OxFX1E() {
    
}
void chip8::OxFX29() {
    
}
void chip8::OxFX33() {
    
}
void chip8::OxFX55() {
    
}
void chip8::OxFX65() {
    
}