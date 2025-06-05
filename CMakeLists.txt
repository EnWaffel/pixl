cmake_minimum_required(VERSION 3.28)

project(pixl VERSION 0.0.1)

# ----- SETTINGS -----

option(PX_VIDEO OFF)
option(PX_3D OFF)
option(PX_TESTS OFF)

# --------------------

# --- Compiler Options / Definitions ---

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(-DPX_DEBUG)
endif()

if(NOT MSVC)
    add_compile_options(-fPIC)
    add_compile_options(-fno-common)
    add_compile_options(-Wno-error=array-bounds) # assimp, why?
endif()

# --------------------------------------

# --- OpenAL ---

set(LIBTYPE STATIC)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_TESTS OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_INSTALL OFF)
set(ALSOFT_INSTALL_CONFIG, OFF)
set(ALSOFT_INSTALL_HRTF_DATA, OFF)
set(ALSOFT_INSTALL_AMBDEC_PRESETS, OFF)
set(ALSOFT_INSTALL_EXAMPLES, OFF)
set(ALSOFT_INSTALL_UTILS, OFF)
set(ALSOFT_UPDATE_BUILD_VERSION, OFF)
add_subdirectory(dependencies/openal-soft)

# --------------

# --- GLFW and Freetype ---

if(WIN32)
    set(BUILD_SHARED_LIBS OFF)
    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_INSTALL OFF)
    add_subdirectory(dependencies/glfw)
else()
    add_subdirectory(dependencies/freetype)
endif()

if(PX_3D)
    set(ASSIMP_BUILD_SAMPLES OFF)
    set(ASSIMP_BUILD_TESTS OFF)
    set(ASSIMP_INSTALL OFF)
    add_subdirectory(dependencies/assimp)
endif()

# -------------------------

add_subdirectory(utility)
add_subdirectory(core)

if(PX_TESTS)
    add_subdirectory(test)
endif()