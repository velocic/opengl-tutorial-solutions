cmake_minimum_required(VERSION 2.8)

project(Tutorial16)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

include_directories("include" "/usr/include/nvidia-370")

add_executable(${PROJECT_NAME}
    src/gl3w.c
    src/tutorial16.cpp
    src/material.cpp
    src/controls.cpp
    src/camera.cpp
    src/utilities.cpp
    src/lodepng.cpp
    src/texture.cpp
)

target_link_libraries(${PROJECT_NAME} SDL2 GL dl)

install(
    TARGETS
        Tutorial16
    DESTINATION
        bin
)
