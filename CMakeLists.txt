cmake_minimum_required(VERSION 2.8)

project(LowPolySDL)

set(BIN_DIR ${LowPolySDL_SOURCE_DIR}/bin)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(${LowPolySDL_SOURCE_DIR}/include)

add_subdirectory(Lesson0)
