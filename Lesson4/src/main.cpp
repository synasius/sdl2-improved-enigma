#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "res_path.h"
#include "cleanup.h"

const auto SCREEN_WIDTH = 640;
const auto SCREEN_HEIGHT = 480;
const auto TILE_SIZE = 40;

/**
 * Log an SDL error with some error message to an output stream
 *
 * @param os The output stream to write the message to.
 * @param msg The error message to write.
 */
void logSDLError(std::ostream& os, const std::string& msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
 * Load an image into a texture on the rendering device.
 *
 * @param file The image file to load.
 * @param renderer The renderer to load the texture onto.
 * @return the loaded the texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* renderer)
{
    auto* texture = IMG_LoadTexture(renderer, file.c_str());
    if (texture == nullptr) {
        logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

/**
 * Draw a texture into a renderer at position x, y with desired
 * width and height.
 */
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y,
                   int w, int h)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

/**
 * Draw a texture into a renderer at position x, y preserving the
 * texture's width and height.
 */
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y)
{
    auto w = 0;
    auto h = 0;
    // initialize width and height from the texture
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    renderTexture(texture, renderer, x, y, w, h);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    // initialize the image subsystem
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        logSDLError(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    // create a window to display stuff
    auto* window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");

        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // create a renderer to draw into the window
    auto* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        cleanup(window);
        logSDLError(std::cout, "CreateRenderer");

        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // load background and foreground images
    auto resPath = getResourcePath("Lesson4");
    auto image = loadTexture(resPath + "image.png", renderer);
    if (image == nullptr) {
        cleanup(image, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // event loop
    SDL_Event e;
    auto quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN) {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);

        // here we render our foreground image at the center of the screen
        auto iW = 0;
        auto iH = 0;
        SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
        auto x = SCREEN_WIDTH / 2 - iW / 2;
        auto y = SCREEN_HEIGHT / 2 - iH / 2;
        renderTexture(image, renderer, x, y);

        SDL_RenderPresent(renderer);
    }

    cleanup(image, renderer, window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
