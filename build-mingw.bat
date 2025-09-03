@echo off
echo --- Building pixl [release]...

if not exist "build\release" (
    mkdir build
    mkdir build\release
)

cd build\release
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" -DPX_VIDEO=ON -DPX_3D=ON ../../
mingw32-make -j8