cmake_minimum_required(VERSION 2.8)

project(Tutorial3)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(PROJECT_SOURCE_DIR "${PROJECT_BINARY_DIR}/src")

include_directories("include")

add_executable(${PROJECT_NAME} src/gl3w.c src/tutorial3.cpp)

target_link_libraries(${PROJECT_NAME} SDL2 GL dl)

install(
    TARGETS
        Tutorial3
    DESTINATION
        bin
)
