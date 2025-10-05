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

# --- fmt ---

set(FMT_DOC OFF)
set(FMT_INSTALL OFF)
set(FMT_TEST OFF)
set(FMT_FUZZ OFF)
set(FMT_CUDA_TEST OFF)
set(FMT_MODULE OFF)
set(FMT_SYSTEM_HEADERS OFF)
set(FMT_UNICODE ON)
add_subdirectory(dependencies/fmt)

# -----------

# --- msdfgen ---

set(MSDFGEN_CORE_ONLY OFF)
set(MSDFGEN_BUILD_STANDALONE OFF)
set(MSDFGEN_USE_VCPKG OFF)
set(MSDFGEN_USE_OPENMP OFF)
set(MSDFGEN_USE_CPP11 ON)
set(MSDFGEN_USE_SKIA OFF)
set(MSDFGEN_DISABLE_SVG ON)
set(MSDFGEN_DISABLE_PNG ON)
set(MSDFGEN_INSTALL ON)
set(MSDFGEN_DYNAMIC_RUNTIME OFF)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(dependencies/msdfgen)

# ---------------

add_subdirectory(utility)
add_subdirectory(core)

if(PX_TESTS)
    add_subdirectory(test)
endif()