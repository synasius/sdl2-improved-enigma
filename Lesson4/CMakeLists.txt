project(Lesson4)

# find the sdl2 image library
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

#
add_executable(Lesson4 src/main.cpp)
target_link_libraries(Lesson4 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
install(TARGETS Lesson4 RUNTIME DESTINATION ${BIN_DIR})
