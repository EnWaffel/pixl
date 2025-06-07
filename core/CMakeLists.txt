cmake_minimum_required(VERSION 3.28)

project(pixl-core VERSION 0.0.1)

add_compile_options(-Wall)
add_compile_definitions(-DPX_EXPORTS)
add_compile_definitions(-DPX_DEBUG)
set(CMAKE_USE_RELATIVE_PATHS TRUE)

set(SRC
src/Core.cpp
src/math/Vec2i.cpp
src/window/Window.cpp
../dependencies/glad/src/glad.c
src/math/Vec2.cpp
src/graphics/RenderPipeline.cpp
src/graphics/Shader.cpp
src/graphics/Framebuffer.cpp
src/math/Vec3.cpp
src/math/Mat4.cpp
src/math/MathFunctions.cpp
src/graphics/DrawingContext.cpp
src/graphics/CameraRenderer.cpp
src/graphics/Camera.cpp
src/graphics/Sprite.cpp
src/asset/AssetPackage.cpp
src/asset/AssetManager.cpp
src/asset/Texture.cpp
src/sound/SoundSystem.cpp
src/asset/AudioBuffer.cpp
src/sound/Sound.cpp
src/animation/loader/SparrowAtlasLoader.cpp
src/animation/Animation.cpp
src/graphics/WindowRenderer.cpp
src/event/EventManager.cpp
src/tween/Tweenable.cpp
src/tween/Easing.cpp
src/input/KeyCode.cpp
src/asset/Font.cpp
src/graphics/Text.cpp
src/graphics/SpriteGroup.cpp
src/graphics/PostProcessingEffect.cpp
)

if(PX_VIDEO)
    list(APPEND SRC src/graphics/VideoPlayer.cpp)
endif()

if(PX_3D)
    list(APPEND SRC
        src/asset/Model.cpp
        src/graphics/Camera3DRenderer.cpp
        src/graphics/Camera3D.cpp
    )
endif()

add_library(pixl-core SHARED ${SRC})

target_include_directories(pixl-core PUBLIC
include/
../utility/include/
../dependencies/glad/include/
../dependencies/stb/
../dependencies/openal-soft/include/
../dependencies/rapidxml/
../dependencies/freetype/include/
../dependencies/utfcpp/source/
../dependencies/assimp/include/
../dependencies/libav/include/
)

if(WIN32)
    target_link_directories(pixl-core PUBLIC ../dependencies/libav/lib/release/ ../dependencies/freetype/lib/release/)
    set(LIBS
    pixl-utility
    glfw
    OpenAL
    freetype
    kernel32
    user32
    gdi32
    winspool
    shell32
    ole32
    oleaut32
    uuid
    comdlg32
    advapi32
    ws2_32
    Mfplat
    Mfuuid
    Secur32
    Bcrypt
    strmiids
    brotlidec
    brotlienc
    brotlicommon
    bz2
    png16
    zlib
    )
else()
    set(LIBS
    pixl-utility
    glfw
    OpenAL
    freetype
    )
    
endif()

if(PX_VIDEO)
    list(APPEND LIBS avcodec avdevice avfilter avformat avutil swresample swscale)
endif()

if(PX_3D)
    list(APPEND LIBS assimp)
endif()

target_link_libraries(pixl-core PUBLIC ${LIBS})