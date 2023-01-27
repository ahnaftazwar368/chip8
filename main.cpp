#include <iostream>
#include "chip8.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Incorrect args given, usage: " << argv[0] << " <ROM> <Delay> "
    }

    Chip8 chip8 {};

    chip8.loadRom(argv[1]);

    bool end = false;

    while (!end) {
        chip8.cycle();
        

    }
}