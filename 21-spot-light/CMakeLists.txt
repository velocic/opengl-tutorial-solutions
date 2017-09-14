cmake_minimum_required(VERSION 2.8)

project(Tutorial20)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

include_directories("include" "/usr/include/nvidia-370")

add_executable(${PROJECT_NAME}
    src/gl3w.c
    src/tutorial20.cpp
    src/controls.cpp
    src/camera.cpp
    src/utilities.cpp
    src/lodepng.cpp
    src/texture.cpp
    src/renderwindow.cpp
    src/materials/material.cpp
    src/materials/phongmaterial.cpp
    src/lights/lightlist.cpp
)

target_link_libraries(${PROJECT_NAME} SDL2 GL dl)

install(
    TARGETS
        Tutorial20
    DESTINATION
        bin
)
