cmake_minimum_required(VERSION 2.8)

project(Tutorial15)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

include_directories("include")

add_executable(${PROJECT_NAME}
    src/gl3w.c
    src/tutorial15.cpp
    src/material.cpp
    src/controls.cpp
    src/camera.cpp
)

target_link_libraries(${PROJECT_NAME} SDL2 GL dl)

install(
    TARGETS
        Tutorial15
    DESTINATION
        bin
)
