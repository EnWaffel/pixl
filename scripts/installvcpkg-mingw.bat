@echo off
echo --- Checking for Git...

where git >nul 2>nul
if %errorlevel% neq 0 (
    echo --- Git not found!
    exit /b 1
)

echo --- Git found

set "folder=temp"

if exist "%folder%" (
    echo --- '%folder%' already exists. If you wish to re-install, please remove the '%folder%' folder.
    exit /b 1
)

echo --- Creating "%folder%"...
mkdir "%folder%"
cd "%folder%"

echo --- Installing vcpkg...

if not exist "vcpkg" (
    call git clone https://github.com/microsoft/vcpkg.git
)

cd vcpkg

echo --- Bootstrapping vcpkg...
call bootstrap-vcpkg.bat -disableMetrics

echo --- Installing freetype...
call vcpkg.exe --disable-metrics install freetype:x64-mingw-static

echo --- Installing ffmpeg...
call vcpkg.exe --disable-metrics install ffmpeg:x64-mingw-static

echo --- Installing zlib...
call vcpkg.exe --disable-metrics install zlib:x64-mingw-static

echo --- Done!
