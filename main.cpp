#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "graphics.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Incorrect args given, usage: " << argv[0] << " <ROM> " << std::endl;
        return 0;
    }
    const char* filename = argv[1];

    // Begin running graphics
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 320;
    std::cout << "Starting Chip-8!" << std::endl;
    Graphics graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Begin chip8
    Chip8 chip8 {};
    chip8.loadRom(filename);

    bool quit = false;

    while (!quit) {
        quit = graphics.handleInput(chip8.keyboard);

        chip8.cycle();

        graphics.updateGraphics(chip8.display);
        std::this_thread::sleep_for(std::chrono::microseconds(1500));
    }
    return 0;
}