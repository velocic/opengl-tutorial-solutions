cmake_minimum_required(VERSION 2.8)

project(Tutorial11)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

include_directories("include")

add_executable(${PROJECT_NAME} src/gl3w.c src/tutorial11.cpp)

target_link_libraries(${PROJECT_NAME} SDL2 GL dl)

install(
    TARGETS
        Tutorial11
    DESTINATION
        bin
)
