cmake_minimum_required(VERSION 3.28)

project(pixl-core VERSION 0.0.1)

add_compile_options(-Wall)
add_compile_definitions(-DPX_EXPORTS)
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
src/graphics/2d/CameraRenderer.cpp
src/graphics/2d/Camera.cpp
src/graphics/2d/Sprite.cpp
src/asset/AssetPackage.cpp
src/asset/AssetManager.cpp
src/asset/Texture.cpp
src/sound/SoundSystem.cpp
src/asset/AudioBuffer.cpp
src/sound/Sound.cpp
src/animation/loader/SparrowAtlasLoader.cpp
src/animation/Animation.cpp
src/graphics/2d/WindowRenderer.cpp
src/event/EventManager.cpp
src/tween/Tweenable.cpp
src/tween/Easing.cpp
src/input/KeyCode.cpp
src/asset/Font.cpp
src/graphics/2d/Text.cpp
src/graphics/2d/SpriteGroup.cpp
src/graphics/PostProcessingEffect.cpp
src/graphics/2d/SimpleRenderer.cpp
src/asset/TextureAtlas.cpp
src/math/Rect.cpp
src/reflect/Field.cpp
src/Shaders.cpp
src/input/Keys.cpp
src/graphics/3d/Skybox.cpp
src/asset/CubeMap.cpp
src/input/Mouse.cpp
)

if(PX_VIDEO)
    list(APPEND SRC src/graphics/2d/VideoPlayer.cpp)
endif()

if(PX_3D)
    add_compile_definitions(-DPX_3D)
    list(APPEND SRC
        src/asset/Model.cpp
        src/graphics/3d/Camera3DRenderer.cpp
        src/graphics/3d/Camera3D.cpp
        src/graphics/3d/Object3D.cpp
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
../dependencies/fmt/include/
../dependencies/glm/
../dependencies/msdfgen/
)

if(WIN32)
    target_link_directories(pixl-core PUBLIC ../dependencies/libav/lib/release/ ../dependencies/freetype/lib/release/)
    set(LIBS
    pixl-utility
    glfw
    OpenAL
    freetype
    brotlidec
    brotlienc
    brotlicommon
    bz2
    png16
    zlib
    fmt
    msdfgen-core
    msdfgen-ext
    )
else()
    set(LIBS
    pixl-utility
    glfw
    OpenAL
    freetype
    fmt
    msdfgen-core
    msdfgen-ext
    )
    
endif()

if(PX_VIDEO)
    list(APPEND LIBS avformat avcodec avdevice avfilter avutil swresample swscale)
endif()

if(WIN32)
    list(APPEND LIBS
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
    )
endif()

if(PX_3D)
    list(APPEND LIBS assimp)
endif()

target_link_libraries(pixl-core PUBLIC ${LIBS})