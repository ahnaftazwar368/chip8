#include <iostream>
#include <chrono>
#include <thread>

#include "chip8.h"
#include "graphics.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Incorrect args given, usage: " << argv[0] << " <ROM> <Delay> " << std::endl;
        return 0;
    }

    // Begin running graphics
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    std::cout << "Starting Chip-8!" << std::endl;
    Graphics graphics(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Begin chip8
    Chip8 chip8 {};

    chip8.loadRom(argv[1]);

    bool quit = false;
    std::vector<int> bufferKeyboard (16, 0);
    while (!quit) {
        quit = graphics.handleInput(chip8.keyboard);

        chip8.cycle();

        graphics.updateGraphics(chip8.display);
        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    }
    return 0;
}