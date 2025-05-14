cmake_minimum_required(VERSION 3.28)

project(pixl LANGUAGES CXX VERSION 0.0.1)

add_compile_options(-fPIC)
add_compile_definitions(-DPX_DEBUG)

add_subdirectory(utility)
add_subdirectory(core)
set(LIBTYPE STATIC)
add_subdirectory(dependencies/openal-soft)
if (WIN32)
add_subdirectory(dependencies/glfw)
endif()
add_subdirectory(dependencies/freetype)