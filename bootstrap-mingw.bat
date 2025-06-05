@echo off

echo --- Preparing...

rmdir /s /q "dependencies\freetype"
rmdir /s /q "dependencies\libav"

echo --- Installing dependencies...
call scripts/installvcpkg-mingw.bat

echo --- Copying files...

:: freetype
mkdir "dependencies\freetype\lib\release"
mkdir "dependencies\freetype\lib\debug"
mkdir "dependencies\freetype\include"

:: include
xcopy "temp\vcpkg\installed\x64-mingw-static\include\freetype"               "dependencies\freetype\include\freetype" /E /I /Y /q
copy /y /v "temp\vcpkg\installed\x64-mingw-static\include\ft2build.h"        "dependencies\freetype\include\ft2build.h"
:: release
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libfreetype.a"         "dependencies\freetype\lib\release\libfreetype.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libpng.a"              "dependencies\freetype\lib\release\libpng.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libpng16.a"            "dependencies\freetype\lib\release\libpng16.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libzlib.a"             "dependencies\freetype\lib\release\libzlib.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libbz2.a"              "dependencies\freetype\lib\release\libbz2.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libbrotlicommon.a"     "dependencies\freetype\lib\release\libbrotlicommon.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libbrotlidec.a"        "dependencies\freetype\lib\release\libbrotlidec.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libbrotlienc.a"        "dependencies\freetype\lib\release\libbrotlienc.a"
:: debug
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libfreetyped.a"  "dependencies\freetype\lib\debug\libfreetyped.a"

:: libav
mkdir "dependencies\libav\lib\release"
mkdir "dependencies\libav\lib\debug"
mkdir "dependencies\libav\include"

:: include
xcopy "temp\vcpkg\installed\x64-mingw-static\include\libavcodec"             "dependencies\libav\include\libavcodec" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-mingw-static\include\libavdevice"            "dependencies\libav\include\libavdevice" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-mingw-static\include\libavfilter"            "dependencies\libav\include\libavfilter" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-mingw-static\include\libavformat"            "dependencies\libav\include\libavformat" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-mingw-static\include\libavutil"              "dependencies\libav\include\libavutil" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-mingw-static\include\libswresample"          "dependencies\libav\include\libswresample" /E /I /Y /q
xcopy "temp\vcpkg\installed\x64-mingw-static\include\libswscale"             "dependencies\libav\include\libswscale" /E /I /Y /q
:: release
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libavcodec.a"          "dependencies\libav\lib\release\libavcodec.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libavdevice.a"         "dependencies\libav\lib\release\libavdevice.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libavfilter.a"         "dependencies\libav\lib\release\libavfilter.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libavformat.a"         "dependencies\libav\lib\release\libavformat.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libavutil.a"           "dependencies\libav\lib\release\libavutil.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libswresample.a"       "dependencies\libav\lib\release\libswresample.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libswscale.a"          "dependencies\libav\lib\release\libswscale.a"
::debug
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libavcodec.a"    "dependencies\libav\lib\debug\libavcodecd.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libavdevice.a"   "dependencies\libav\lib\debug\libavdeviced.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libavfilter.a"   "dependencies\libav\lib\debug\libavfilterd.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libavformat.a"   "dependencies\libav\lib\debug\libavformatd.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libavutil.a"     "dependencies\libav\lib\debug\libavutild.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libswresample.a" "dependencies\libav\lib\debug\libswresampled.a"
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libswscale.a"    "dependencies\libav\lib\debug\libswscaled.a"

:: zlib
mkdir "dependencies\zlib\lib\release"
mkdir "dependencies\zlib\lib\debug"

:: release
copy /y /v "temp\vcpkg\installed\x64-mingw-static\lib\libzlib.a"             "dependencies\zlib\lib\release\libzlib.a"
:: debug
copy /y /v "temp\vcpkg\installed\x64-mingw-static\debug\lib\libzlibd.a"             "dependencies\zlib\lib\release\libzlibd.a"

echo --- Done! Now you can run the build.bat script for a release build or the buildd.bat for a debug build.