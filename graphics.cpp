#include <iostream>
#include "graphics.h"

Graphics::Graphics(const int width, const int height) {
    //Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;

    //Create window
    window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        width, height, SDL_WINDOW_SHOWN);
    if(window == NULL)
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) 
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
    // SDL_RenderSetLogicalSize(renderer, width, height);
    
    // Create texture with little endian formatting
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING, 64, 32);
    if (texture == NULL)
        std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
}

Graphics::~Graphics() {
    std::cout << "Shutting down :(" << std::endl;

    //Destroy SDL items
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    //Quit SDL subsystems
    SDL_Quit();
}

void Graphics::updateGraphics(void *pixels) {
    // update texture with new data
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);    
}

bool Graphics::handleInput(uint8_t* keyboard) {
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
                break;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_1:
                        keyboard[0] = 1;
                        std::cout << "Button Press!" << std::endl;
                        break;
                    case SDLK_2:
                        keyboard[1] = 1;
                        break;
                    case SDLK_3:
                        keyboard[2] = 1;
                        break;
                    case SDLK_4:
                        keyboard[3] = 1;
                        break;
                    case SDLK_q:
                        keyboard[4] = 1;
                        break;
                    case SDLK_w:
                        keyboard[5] = 1;
                        break;
                    case SDLK_e:
                        keyboard[6] = 1;
                        break;
                    case SDLK_r:
                        keyboard[7] = 1;
                        break;
                    case SDLK_a:
                        keyboard[8] = 1;
                        break;
                    case SDLK_s:
                        keyboard[9] = 1;
                        break;
                    case SDLK_d:
                        keyboard[10] = 1;
                        break;
                    case SDLK_f:
                        keyboard[11] = 1;
                        break;
                    case SDLK_z:
                        keyboard[12] = 1;
                        break;
                    case SDLK_x:
                        keyboard[13] = 1;
                        break;
                    case SDLK_c:
                        keyboard[14] = 1;
                        break;
                    case SDLK_v:
                        keyboard[15] = 1;
                        break;
                    default:
                        continue;
                        break;
                }
            }
        }
    }
    return (quit == true);
}
