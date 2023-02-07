#include <iostream>
#include <fstream>
#include <algorithm>
#include "chip8.h"

// Initialise the entire Chip8 system by loading the first ROM instruction
// stored at 0x200
Chip8::Chip8() {
    pc = ROMSTART;
    sp = 0;
    indexRego = 0;
    opcode = 0;
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
void Chip8::loadRom(const char* romFile) {
    std::ifstream file(romFile, std::ios::binary);

    char* buffer{};
    if (file.is_open()) {
        // find the size of the current file and create a buffer for the value
        file.seekg(0, file.end);
        int fileSize = file.tellg();
        buffer = new char[fileSize];

        // rewind the file and then copy all contents to buffer
        file.seekg(0, file.beg);
        file.read(buffer, fileSize);
        file.close();

        // load buffer to memory (starting from 0x200)
        for (int i = 0; i < fileSize; i++) {
            Chip8::memory[ROMSTART + i] = buffer[i];
        }
    } else {
        std::cerr << "men are my preference" << std::endl;
    }

    delete[] buffer;
}

// Random num generator
int Chip8::giveRandInt() {
    std::srand((unsigned) time(NULL));
    return rand() % 256;
}

void Chip8::cycle() {
    // fetch
    opcode = (memory[pc] << 8) | memory[pc+1];
    pc += 2;
    // decode 
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    Chip8::Ox00E0();
                    break;
                case 0x000E:
                    Chip8::Ox00EE();
                    break;

                default:
                    std::cerr << "OpCode error, program crashed!";
            }
            break;
        case 0x1000:
            Chip8::Ox1NNN();
            break;
        case 0x2000:
            Chip8::Ox2NNN();
            break;
        case 0x3000:
            Chip8::Ox3XKK();
            break;
        case 0x4000:
            Chip8::Ox4XKK();
        case 0x5000:
            Chip8::Ox5XY0();
            break;
        case 0x6000:
            Chip8::Ox6XKK();
            break;
        case 0x7000:
            Chip8::Ox7XKK();
            break;
        case 0x9000:
            Chip8::Ox9XY0();
            break;
        case 0xA000:
            Chip8::OxANNN();
            break;
        case 0xB000:
            Chip8::OxBNNN();
            break;
        case 0xC000:
            Chip8::OxCXKK();
            break;
        case 0xD000:
            Chip8::OxDXYN();
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:
                    Chip8::Ox8XY0();
                    break;
                case 0x0001:
                    Chip8::Ox8XY1();
                    break;
                case 0x0002:
                    Chip8::Ox8XY2();
                    break;
                case 0x0003:
                    Chip8::Ox8XY3();
                    break;
                case 0x0004:
                    Chip8::Ox8XY4();
                    break;
                case 0x0005:
                    Chip8::Ox8XY5();
                    break;
                case 0x0006:
                    Chip8::Ox8XY6();
                    break;
                case 0x0007:
                    Chip8::Ox8XY7();
                    break;
                case 0x000E:
                    Chip8::Ox8XYE();
                    break;
                
                default:
                    std::cerr << "OpCode error, program crashed!";
                }
            break;
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x00A1:
                    Chip8::OxEXA1();
                    break;
                case 0x009E:
                    Chip8::OxEX9E();
                    break;

                default:
                    std::cerr << "OpCode error, program crashed!";
                }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:
                    Chip8::OxFX07();
                    break;
                case 0x000A:
                    Chip8::OxFX0A();
                    break;
                case 0x0015:
                    Chip8::OxFX15();
                    break;
                case 0x0018:
                    Chip8::OxFX18();
                    break;
                case 0x001E:
                    Chip8::OxFX1E();
                    break;
                case 0x0029:
                    Chip8::OxFX29();
                    break;
                case 0x0033:
                    Chip8::OxFX33();
                    break;
                case 0x0055:
                    Chip8::OxFX55();
                    break;
                case 0x0065:
                    Chip8::OxFX65();
                    break;

                default:
                    break;
                }
            break;
        default:
            std::cerr << "OpCode error, program crashed!";
    }

    if (delayTimer > 0) {
        delayTimer--;
    }

    if (soundTimer > 0) {
        soundTimer--;
    }
}

// All Opcodes
void Chip8::Ox00E0() {
    std::fill(display, display + (64*32), 0);
}

void Chip8::Ox00EE() {
    sp--;
    pc = stack[sp];
}

void Chip8::Ox1NNN() {
    // Opcode is made of 16 bits, 4 of which are used to give instruction of
    // which exact instruction is being called while the following 12 bits
    // give any operands that are contained in the opcode. Hence the bitwise
    // below gets rid of the first 4 bytes and captures the target address.
    uint16_t address = opcode & 0x0FFF;
    pc = address;
}

void Chip8::Ox2NNN() {
    uint16_t address = opcode & 0x0FFF;
    stack[sp] = pc;
    sp++;
    pc = address;
}

void Chip8::Ox3XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = opcode & 0x00FF;

    if (registers[targetRego] == targetValue) {
        pc += 2;
    }
}

void Chip8::Ox4XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = opcode & 0x00FF;

    if (registers[targetRego] != targetValue) {
        pc += 2;
    }
}

void Chip8::Ox5XY0() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    if (registers[targetRegoX] == registers[targetRegoY]) {
        pc += 2;
    }
}

void Chip8::Ox6XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = (opcode & 0x00FF);

    registers[targetRego] = targetValue;
}

void Chip8::Ox7XKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint8_t targetValue = (opcode & 0x00FF);

    registers[targetRego] += targetValue;
}

void Chip8::Ox8XY0() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoY];
}

void Chip8::Ox8XY1() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoX] | registers[targetRegoY];
}

void Chip8::Ox8XY2() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoX] & registers[targetRegoY];
}

void Chip8::Ox8XY3() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    registers[targetRegoX] = registers[targetRegoX] ^ registers[targetRegoY];
}

void Chip8::Ox8XY4() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    int sum = registers[targetRegoX] + registers[targetRegoY];

    registers[FLAGREGO] = sum > 255 ? 1 : 0;

    registers[targetRegoX] = sum & 0x00FF;
}

void Chip8::Ox8XY5() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    int diff = registers[targetRegoX] - registers[targetRegoY];

    registers[FLAGREGO] = diff > 0 ? 1 : 0;

    registers[targetRegoX] = diff;
}

void Chip8::Ox8XY6() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;

    if (registers[targetRego] & 0x0001) {
        registers[FLAGREGO] = 1;
    } else {
        registers[FLAGREGO] = 0;
        registers[targetRego] /= 2;
    }
}

void Chip8::Ox8XY7() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    int diff = registers[targetRegoX] - registers[targetRegoY];

    registers[FLAGREGO] = diff < 0 ? 1 : 0;

    registers[targetRegoX] = registers[targetRegoY] - registers[targetRegoX];
}

void Chip8::Ox8XYE() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;

    registers[FLAGREGO] = registers[targetRego] & 0x8000 ? 1 : 0;

    registers[targetRego] *= 2;
}

void Chip8::Ox9XY0() {
    uint16_t targetRegoX = (opcode & 0x0F00) >> 8;
    uint16_t targetRegoY = (opcode & 0x00F0) >> 4;

    if (registers[targetRegoX] != registers[targetRegoY]) {
        pc += 2;
    }
}

void Chip8::OxANNN() {
    uint16_t address = opcode & 0x0FFF;
    indexRego = address;
}

void Chip8::OxBNNN() {
    uint16_t address = opcode & 0x0FFF;
    pc = address + registers[0];
}

void Chip8::OxCXKK() {
    uint16_t targetRego = (opcode & 0x0F00) >> 8;
    uint16_t targetValue = (opcode & 0x00FF);

    registers[targetRego] = giveRandInt() & targetValue;
}

void Chip8::OxDXYN() {
    uint16_t targetRegoX = opcode & 0x0F00 >> 8;
    uint16_t targetRegoY = opcode & 0x00F0 >> 4;
    // since each byte represents 8 bits and each column of data
    // will be in 8 bits, we can assert that the total height of 
    // the item to print will be numBytes and the width will be 8
    uint16_t numBytes = opcode & 0x000F;

    // Only starting pos should wrap about display, not overflowing
    // sprites
    uint8_t startX = registers[targetRegoX] % MAXWIDTH;
    uint8_t startY = registers[targetRegoY] % MAXHEIGHT;

    for (uint row = 0; row < numBytes; row++) {
        // Graphics for sprites are loaded by 8 bit rows starting from
        // the address stored at the index register
        uint8_t currentRow = memory[indexRego + row];
        for (uint col = 0; col < 8; col++) {
            uint32_t* currentDisplayPixel = &display[(startX+col) + MAXWIDTH*(startY+row)];
            if (currentRow & (0x8000 >> (MAXWIDTH-col))) {
                // Set the flag variable in the event that any pixels are erased
                if (*currentDisplayPixel == 0xFFFFFFFF) {
                    registers[FLAGREGO] = 1;
                } else {
                    registers[FLAGREGO] = 0;
                }
                *currentDisplayPixel = *currentDisplayPixel ^ 0xFFFFFFFF;
            } else if (*currentDisplayPixel == 0) {
                registers[FLAGREGO] = 0;
            }
        }
    } 
}

void Chip8::OxEX9E() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;
    uint8_t targetKey = registers[targetRego];

    if (keyboard[targetKey] == 1) {
        pc += 2;
    }
}

void Chip8::OxEXA1() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;
    uint8_t targetKey = registers[targetRego];

    if (keyboard[targetKey] != 1) {
        pc += 2;
    }
}

void Chip8::OxFX07() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;
    
    registers[targetRego] = delayTimer;
}

void Chip8::OxFX0A() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    // check if any keys have been pressed on each cycle of fetch/ execute
    int i {};
    for (i = 0; i < 16; i++) {
        if (keyboard[i] == 1) {
            registers[targetRego] = i;
            break;
        }
    }
    // repeat if no keypress by decrementing pc back to this instruction
    if (i == 16) pc -= 2;
}

void Chip8::OxFX15() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    delayTimer = registers[targetRego];
}

void Chip8::OxFX18() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    soundTimer = registers[targetRego];
}

void Chip8::OxFX1E() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    indexRego += registers[targetRego];
}

void Chip8::OxFX29() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    // each sprite is saved as 8 bit lines with a height of 5 bytes
    indexRego = FONTSTART + 5 * registers[targetRego];
}

void Chip8::OxFX33() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    memory[indexRego] = registers[targetRego] / 100;
    memory[indexRego + 1] =  (registers[targetRego] % 100) / 10;
    memory[indexRego + 2] = (registers[targetRego] % 10);
}

void Chip8::OxFX55() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    for (auto i = 0; i <= targetRego; i++) {
        memory[indexRego + i] = registers[i];
    }
}

void Chip8::OxFX65() {
    uint16_t targetRego = opcode & 0x0F00 >> 8;

    for (auto i = 0; i <= targetRego; i++) {
        registers[i] = memory[indexRego + i];
    }
}

void Chip8::invalidOp() {
    // do nothing if invalid op
}