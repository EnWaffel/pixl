@echo off
echo --- Building pixl [release]...

if not exist "build\release" (
    mkdir build
    mkdir build\release
)

cd build\release
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ../../
mingw32-make