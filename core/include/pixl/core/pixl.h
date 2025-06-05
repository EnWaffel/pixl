#pragma once

// utility
#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/utility/Axis.h"
#include "pixl/utility/Timer.h"
#include "pixl/utility/Color.h"

// core
#include "pixl/core/animation/loader/SparrowAtlasLoader.h"
#include "pixl/core/animation/Animation.h"
#include "pixl/core/asset/AssetManager.h"
#include "pixl/core/asset/AssetPackage.h"
#include "pixl/core/asset/AudioBuffer.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/core/event/Event.h"
#include "pixl/core/event/EventManager.h"
#include "pixl/core/graphics/Camera.h"
#include "pixl/core/graphics/CameraRenderer.h"
#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/graphics/DrawingContext.h"
#include "pixl/core/graphics/Framebuffer.h"
#include "pixl/core/graphics/PipelineElement.h"
#include "pixl/core/graphics/RenderPipeline.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/graphics/Sprite.h"
#include "pixl/core/graphics/WindowRenderer.h"
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
#include "pixl/core/graphics/Text.h"
#include "pixl/core/asset/Font.h"
#include "pixl/core/graphics/VideoPlayer.h"
#include "pixl/core/graphics/SpriteGroup.h"

#define PX_ERROR_GLFW 0x20
#define PX_ERROR_SERVICE 0x21

namespace px
{
    struct PX_API InitOptions
    {
        bool errorLogging = true;
    };

    // Library functions
    PX_API Error Init(const InitOptions& options = {});
    PX_API void End();

    // Window functions
    PX_API WINDOW CreateWindow(CREFSTR title, const Vec2i& size, bool startVisible = false, bool decorated = true);

    // Utility functions
    PX_API float GetTime();
}