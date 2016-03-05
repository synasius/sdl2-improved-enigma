#ifndef RES_PATH_H
#define RES_PATH_H

#include <iostream>
#include <string>
#include <SDL.h>

/**
 * Get the resource path for resources located in res/subDir
 * It's assumed the project directory is structured like:
 *
 * project/
 *   bin/
 *     executable
 *   res/
 */
std::string getResourcePath(const std::string &subDir = "")
{
    // Choose the right path separator based on the platform
    // we're running on.
    // TODO: check if it is worth using boost::filesystem
    //       On a side note, C++17 std::fs is based on boost::filesystem
#ifdef _WIN32
    const char PATH_SEP = '\\';
#else
    const char PATH_SEP = '/';
#endif

    static std::string baseRes;
    if (baseRes.empty()) {
        // SDL_GetBasePath returns the path to the executable.
        // It returns NULL if something went wrong.
        char* basePath = SDL_GetBasePath();
        if (basePath) {
            baseRes = basePath;
            SDL_free(basePath);
        } else {
            std::cerr << "Error getting resource path: " <<
                SDL_GetError() << std::endl;
        }

        // We replace the last bin/ with res/ to get the resource path
        size_t pos = baseRes.find("bin");
        baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
    }

    return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

#endif /* RES_PATH_H */
