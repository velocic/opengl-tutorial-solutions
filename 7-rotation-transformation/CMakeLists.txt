cmake_minimum_required(VERSION 2.8)

project(Tutorial7)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

include_directories("include")

add_executable(${PROJECT_NAME} src/gl3w.c src/tutorial7.cpp)

target_link_libraries(${PROJECT_NAME} SDL2 GL dl)

install(
    TARGETS
        Tutorial7
    DESTINATION
        bin
)
