cmake_minimum_required(VERSION 3.5)

project(Tutorial22)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -g")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

find_package(PkgConfig)

pkg_search_module(ASSIMP REQUIRED assimp)

add_executable(${PROJECT_NAME}
    src/gl3w.c
    src/tutorial22.cpp
    src/controls.cpp
    src/camera.cpp
    src/utilities.cpp
    src/lodepng.cpp
    src/texture.cpp
    src/renderwindow.cpp
    src/materials/material.cpp
    src/materials/phongmaterial.cpp
    src/lights/lightlist.cpp
    src/asset/mesh.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    SDL2
    GL
    dl
    ${ASSIMP_LIBRARIES}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    "include"
)

install(
    TARGETS
        Tutorial22
    DESTINATION
        bin
)
