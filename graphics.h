#include <SDL2/SDL.h>
#include <vector>

#ifndef GRAPHICS_H
#define  GRAPHICS_H

class Graphics {
public:
    // Constructors and destructors
    Graphics(const int width, const int height);
    ~Graphics();
    // member funcs
    bool handleInput(std::vector<int>& keyboard);
    int currentKeyDown;
private:
    SDL_Window* window;
    SDL_Event event;
};

#endif