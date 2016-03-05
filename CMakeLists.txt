cmake_minimum_required(VERSION 3.1)

# set the default c++ standard required
# to build the project.
set(CMAKE_CXX_STANDARD 11)

project(LowPolySDL)

set(BIN_DIR ${LowPolySDL_SOURCE_DIR}/bin)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(${LowPolySDL_SOURCE_DIR}/include)

add_subdirectory(Lesson0)
