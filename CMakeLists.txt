cmake_minimum_required(VERSION "3.12")
project(dino-game-copy)

add_executable(dino-game "./source/main.cpp" "./source/base.cpp" "./source/headers/base.h")

include_directories(dino-game "${CMAKE_CURRENT_SOURCE_DIR}/source/")