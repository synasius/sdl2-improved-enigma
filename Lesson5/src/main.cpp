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
 * Draw a texture into a renderer at some destination rect
 * taking a clip of the texture if desired.
 */
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Rect dst,
                   SDL_Rect* clip = nullptr)
{
    SDL_RenderCopy(renderer, texture, clip, &dst);
}

/**
 * Draw a texture into a renderer at position x, y preserving the
 * texture's width and height.
 */
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y,
                   SDL_Rect* clip = nullptr)
{
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    if (clip != nullptr) {
        dest.w = clip->w;
        dest.h = clip->h;
    } else {
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    }
    renderTexture(texture, renderer, dest, clip);
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
    auto resPath = getResourcePath("Lesson5");
    auto image = loadTexture(resPath + "image.png", renderer);
    if (image == nullptr) {
        cleanup(image, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Here we compute the position of our clipped image which is 100x100
    auto iW = 100;
    auto iH = 100;
    auto x = SCREEN_WIDTH / 2 - iW / 2;
    auto y = SCREEN_HEIGHT / 2 - iH / 2;
    // the we precalculate the clip rects
    SDL_Rect clips[4];
    for(auto i = 0; i < 4; ++i) {
        clips[i].x = i % 2 * iW;
        clips[i].y = i / 2 * iH;
        clips[i].w = iW;
        clips[i].h = iH;
    }

    // event loop
    SDL_Event e;
    auto quit = false;
    auto useClip = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                        useClip = 0;
                        break;
                    case SDLK_2:
                        useClip = 1;
                        break;
                    case SDLK_3:
                        useClip = 2;
                        break;
                    case SDLK_4:
                        useClip = 3;
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);

        renderTexture(image, renderer, x, y, &clips[useClip]);

        SDL_RenderPresent(renderer);
    }

    cleanup(image, renderer, window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
