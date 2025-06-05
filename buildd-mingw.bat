@echo off
echo --- Building pixl [debug]...

if not exist "build\debug" (
    mkdir build
    mkdir build\debug
)

cd build\debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ../../
mingw32-make