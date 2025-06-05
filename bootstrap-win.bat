@echo off

echo --- Preparing...

rmdir /s /q "dependencies\freetype"
rmdir /s /q "dependencies\libav"

echo --- Installing dependencies...
call scripts/installvcpkg-windows.bat

echo --- Copying files...

:: freetype
mkdir "dependencies\freetype\lib\release"
mkdir "dependencies\freetype\lib\debug"
mkdir "dependencies\freetype\include"

:: include
xcopy "temp\vcpkg\installed\x64-windows-static\include\freetype"               "dependencies\freetype\include\freetype" /E /I /Y /q
copy /y /v "temp\vcpkg\installed\x64-windows-static\include\ft2build.h"        "dependencies\freetype\include\ft2build.h"
:: release
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libfreetype.lib"         "dependencies\freetype\lib\release\libfreetype.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libpng.lib"              "dependencies\freetype\lib\release\libpng.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libpng16.lib"            "dependencies\freetype\lib\release\libpng16.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libzlib.lib"             "dependencies\freetype\lib\release\libzlib.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libbz2.lib"              "dependencies\freetype\lib\release\libbz2.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libbrotlicommon.lib"     "dependencies\freetype\lib\release\libbrotlicommon.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libbrotlidec.lib"        "dependencies\freetype\lib\release\libbrotlidec.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\libbrotlienc.lib"        "dependencies\freetype\lib\release\libbrotlienc.lib"
:: debug
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\freetyped.lib"  "dependencies\freetype\lib\debug\libfreetyped.lib"

:: libav
mkdir "dependencies\libav\lib\release"
mkdir "dependencies\libav\lib\debug"
mkdir "dependencies\libav\include"

:: include
xcopy "temp\vcpkg\installed\x64-windows-static\include\libavcodec"             "dependencies\libav\include\libavcodec" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-windows-static\include\libavdevice"            "dependencies\libav\include\libavdevice" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-windows-static\include\libavfilter"            "dependencies\libav\include\libavfilter" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-windows-static\include\libavformat"            "dependencies\libav\include\libavformat" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-windows-static\include\libavutil"              "dependencies\libav\include\libavutil" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-windows-static\include\libswresample"          "dependencies\libav\include\libswresample" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-windows-static\include\libswscale"             "dependencies\libav\include\libswscale" /E /I /Y /q
:: release
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\avcodec.lib"          "dependencies\libav\lib\release\libavcodec.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\avdevice.lib"         "dependencies\libav\lib\release\libavdevice.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\avfilter.lib"         "dependencies\libav\lib\release\libavfilter.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\avformat.lib"         "dependencies\libav\lib\release\libavformat.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\avutil.lib"           "dependencies\libav\lib\release\libavutil.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\swresample.lib"       "dependencies\libav\lib\release\libswresample.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\swscale.lib"          "dependencies\libav\lib\release\libswscale.lib"
::debug
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\avcodec.lib"    "dependencies\libav\lib\debug\libavcodecd.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\avdevice.lib"   "dependencies\libav\lib\debug\libavdeviced.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\avfilter.lib"   "dependencies\libav\lib\debug\libavfilterd.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\avformat.lib"   "dependencies\libav\lib\debug\libavformatd.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\avutil.lib"     "dependencies\libav\lib\debug\libavutild.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\swresample.lib" "dependencies\libav\lib\debug\libswresampled.lib"
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\swscale.lib"    "dependencies\libav\lib\debug\libswscaled.lib"

:: zlib
mkdir "dependencies\zlib\lib\release"
mkdir "dependencies\zlib\lib\debug"

:: release
copy /y /v "temp\vcpkg\installed\x64-windows-static\lib\zlib.lib"             "dependencies\zlib\lib\release\libzlib.lib"
:: debug
copy /y /v "temp\vcpkg\installed\x64-windows-static\debug\lib\zlibd.lib"      "dependencies\zlib\lib\release\libzlibd.lib"

echo --- Done! Now you can run the build.bat script for a release build or the buildd.bat for a debug build.