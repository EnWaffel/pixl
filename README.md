# pixl

![nevergonnagiveyouup](https://img.shields.io/badge/status-active-brightgreen) [![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#) [![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)](#) [![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)](#)


## Introduction

pixl is a (simple) 2D game engine / library made for C++. It currently supports Windows, Linux and macOS in the future

Documentation can be found [here](https://github.com/EnWaffel/pixl/wiki).

## Features

- 2D Graphics (OpenGL)
- 3D Graphics and Models (OpenGL and assimp)  !!! W.I.P !!!
- Sound (OpenAL)
- Asset packages (APKG)
- Post Processing
- Tweening
- Multiple windows
- Fully integrated Video Player (libav / ffmpeg)
- And more!

### Toggleable features

Some features of pixl need to be enabled during compilation.  
Just add the following arguments to the CMake command to enable the features you want:
- -DPX_VIDEO | For the Video Player
- -DPX_3D | For 3D Graphics

## Compiling pixl

### Requirements:

- Git
- CMake
- MinGW or MSVC

### All you need to do is:

1. Clone this repository (with the submodules!)
```bash
git clone --recurse-submodules https://github.com/EnWaffel/pixl.git
```

2. Run one of the bootstrap scripts (currently only for Windows)
```bash
# Windows
bootstrap-mingw.bat
bootstrap-win.bat
```

3. Run one of the build scripts
```bash
# Linux
./build.sh
./buildd.sh

# Windows
build-mingw.bat
buildd-mingw.bat
build-win.bat # MSVC aka Visual Studio
buildd-win.bat # MSVC aka Visual Studio
```

4. Wait for it to build and enjoy!

## Using pixl in a project

To use pixl in a project you can add it as a sub directory to a CMake project or just link the shared library. pixl is designed to be a all in one library. (All core functionality in one file)

### Debugging

If you want to enable debug logging for pixl, you need to compile pixl with
```PX_DEBUG```.