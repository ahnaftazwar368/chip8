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
    void updateGraphics(void *pixels);
    bool handleInput(uint8_t *keyboard);
    int currentKeyDown;
private:
    SDL_Window* window;
    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};

#endif