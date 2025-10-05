#pragma once

// utility
#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/utility/Axis.h"
#include "pixl/utility/Timer.h"
#include "pixl/utility/Color.h"
#include "pixl/utility/FileUtils.h"

// core
#include "pixl/core/animation/loader/SparrowAtlasLoader.h"
#include "pixl/core/animation/Animation.h"
#include "pixl/core/asset/AssetManager.h"
#include "pixl/core/asset/AssetPackage.h"
#include "pixl/core/asset/AudioBuffer.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/core/event/Event.h"
#include "pixl/core/event/EventManager.h"
#include "pixl/core/graphics/2d/Camera.h"
#include "pixl/core/graphics/2d/CameraRenderer.h"
#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/graphics/DrawingContext.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/graphics/PipelineElement.h"
#include "pixl/core/graphics/RenderPipeline.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/2d/Sprite.h"
#include "pixl/core/graphics/2d/WindowRenderer.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/core/math/MathFunctions.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Vec2i.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/core/sound/Sound.h"
#include "pixl/core/sound/SoundGroup.h"
#include "pixl/core/sound/SoundSystem.h"
#include "pixl/core/window/Window.h"
#include "pixl/core/tween/Tween.h"
#include "pixl/core/tween/Tweenable.h"
#include "pixl/core/tween/Easing.h"
#include "pixl/core/graphics/2d/Text.h"
#include "pixl/core/asset/Font.h"
#include "pixl/core/graphics/2d/VideoPlayer.h"
#include "pixl/core/graphics/2d/SpriteGroup.h"
#include "pixl/core/graphics/PostProcessingEffect.h"
#include "pixl/core/graphics/2d/SimpleRenderer.h"
#include "pixl/core/input/Keys.h"
#include "pixl/core/input/Mouse.h"

#define PX_ERROR_GLFW 0x20
#define PX_ERROR_SERVICE 0x21
#define PX_ERROR_OPENGL 0x22

namespace px
{
    struct InitOptions
    {
        bool errorLogging = true;
    };

    struct EngineOptions
    {
        
    };

    /**
     * @brief Initializes the library. This function needs to be called before any other library function!
     * 
     * @param options Optional parameters for pixl. (e.g. Whether errors should be logged to the console)
     * 
     * @return PX_SUCCESS if no error occurred, PX_ERROR_GLFW for GLFW errors or PX_ERROR_SERVICE for internal service errors.
     */
    PX_API Error Init(const InitOptions& options = {});
    /**
     * @brief Terminates pixl and frees any resources allocated by the library.
     */
    PX_API void End();

    /**
     * @brief Creates a window for rendering.
     * 
     * @param title The title of the window.
     * @param size The width and height of the window in pixels.
     * @param startVisible Whether the window should be visible after creation.
     * @param decorated Whether to have top bar, close and minimize / maximize buttons.
     * 
     * @return A pointer to the Window object.
     */
    PX_API WINDOW CreateWindow(CREFSTR title, const Vec2i& size, bool startVisible = false, bool decorated = true);

    /**
     * @brief Returns the time since pixl was initialized in seconds.
     * 
     * @return The time since initialization in seconds.
     */
    PX_API float GetTime();
}