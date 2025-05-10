# pixl

![nevergonnagiveyouup](https://img.shields.io/badge/status-active-brightgreen) [![Windows](https://custom-icon-badges.demolab.com/badge/Windows-0078D6?logo=windows11&logoColor=white)](#) [![Linux](https://img.shields.io/badge/Linux-FCC624?logo=linux&logoColor=black)](#) [![macOS](https://img.shields.io/badge/macOS-000000?logo=apple&logoColor=F0F0F0)](#)

## Introduction

pixl is a (simple) 2D game engine / library made for C++. It currently supports Windows, Linux and macOS (although macOS was not tested yet)

Documentation can be found [here](/).

## Compiling pixl

### All you need to do is:

1. Clone this repository (with the submodules!)
```bash
git clone --recurse-submodules https://github.com/EnWaffel/pixl.git
```
2. Run the build script
```bash
./build.sh # Or for Windows: build.bat
```
3. Wait for it to build and enjoy!

## Using pixl in a project

To use pixl in a project you can add it as a sub directory to a CMake project or just link to the libpixl-core... file. All functionality of the base system of pixl is in the one shared library.

### Debugging

If you want to enable debug logging for pixl, you need to compile pixl with
```PX_DEBUG```.