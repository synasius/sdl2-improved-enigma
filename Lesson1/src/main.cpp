#include <iostream>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // create a window to display stuff
    auto* window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // create a renderer to draw into the window
    auto* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        cleanup(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return 1;
    }

    // load an image from file into a SDL_Surface
    auto imagePath = getResourcePath("Lesson1") + "hello.bmp";
    auto* image = SDL_LoadBMP(imagePath.c_str());
    if (image == nullptr) {
        cleanup(window, renderer);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;

        SDL_Quit();
        return 1;
    }

    // load the SDL_Surface to the rendering context
    auto* texture = SDL_CreateTextureFromSurface(renderer, image);
    // once loaded the image in memory can be released
    cleanup(image);
    if (texture == nullptr) {
        cleanup(window, renderer);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // draw the texture
    for (int i = 0; i < 3; ++i) {
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_RenderPresent(renderer);

        SDL_Delay(1000);
    }

    cleanup(window, renderer, texture);
    SDL_Quit();
    return 0;
}
