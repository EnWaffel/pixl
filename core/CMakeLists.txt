cmake_minimum_required(VERSION 3.28)

project(pixl-core VERSION 0.0.1)

add_compile_options(-Wall)

add_library(pixl-core SHARED
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
)

target_include_directories(pixl-core PUBLIC include/ ../utility/include/ ../dependencies/glad/include/ ../dependencies/stb/ ../dependencies/openal-soft/include/ ../dependencies/rapidxml/)
target_link_libraries(pixl-core PUBLIC pixl-utility glfw OpenAL)