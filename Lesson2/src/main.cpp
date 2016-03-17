#include <iostream>
#include <SDL.h>
#include "res_path.h"
#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
 * Load a BMP image into a texture on the rendering device.
 *
 * @param file The BMP image.
 * @param renderer The renderer to load the texture onto.
 * @return the loaded the texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* renderer)
{
    SDL_Texture* texture = nullptr;
    // Load the image
    SDL_Surface* image = SDL_LoadBMP(file.c_str());
    if (image != nullptr) {
        texture = SDL_CreateTextureFromSurface(renderer, image);
        cleanup(image);  // remove the image once loaded into a texture

        if (texture == nullptr) {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    } else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

/**
 * Draw a texture into a renderer at position x, y preserving the
 * texture's width and height.
 */
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    // initialize width and height from the texture
    SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    // create a window to display stuff
    auto* window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    // create a renderer to draw into the window
    auto* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        cleanup(window);
        logSDLError(std::cout, "CreateRenderer");

        SDL_Quit();
        return 1;
    }

    // load background and foreground images
    auto resPath = getResourcePath("Lesson2");
    auto background = loadTexture(resPath + "background.bmp", renderer);
    auto image = loadTexture(resPath + "image.bmp", renderer);
    if (background == nullptr || image == nullptr) {
        cleanup(background, image, renderer, window);
        SDL_Quit();
        return 1;
    }

    //
    for (int i = 0; i < 3; ++i) {
        SDL_RenderClear(renderer);

        // here we render a tiled background
        auto bW = 0;
        auto bH = 0;
        SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
        for (auto x = 0; x < SCREEN_WIDTH; x += bW) {
            for(auto y = 0; y < SCREEN_HEIGHT; y += bH) {
                renderTexture(background, renderer, x, y);
            }
        }

        // here we render our foreground image at the center of the screen
        auto iW = 0;
        auto iH = 0;
        SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
        auto x = SCREEN_WIDTH / 2 - iW / 2;
        auto y = SCREEN_HEIGHT / 2 - iH / 2;
        renderTexture(image, renderer, x, y);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    cleanup(background, image, renderer, window);
    SDL_Quit();
    return 0;
}
