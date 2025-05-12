cmake_minimum_required(VERSION 3.28)

project(pixl-utility LANGUAGES CXX VERSION 0.0.1)

add_compile_options(-Wall)
add_compile_definitions(-DPX_EXPORTS)

add_library(pixl-utility STATIC src/Error.cpp src/Color.cpp src/Timer.cpp src/Debug.cpp)
target_include_directories(pixl-utility PUBLIC include/)