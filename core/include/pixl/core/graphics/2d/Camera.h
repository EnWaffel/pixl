#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/tween/Tweenable.h"

#include <vector>

namespace px
{
    class Window;
    typedef Window* WINDOW;

    class Camera : public Tweenable
    {
    public:
        Vec2 pos;
        float rotation = 0.0f;
        float zoom = 1.0f;
        bool visible = true;
        bool active = true;
    public:
        PX_API Camera(WINDOW window);

        PX_API void Update(float delta);
        PX_API void Draw(DRAWINGCTX ctx, SHADER* shaders);
        PX_API void Add(DRAWABLE drawable);
        PX_API void Add(Drawable& drawable);
        PX_API void Remove(DRAWABLE drawable);
        PX_API void Remove(Drawable& drawable);
        PX_API void Reorder();
        PX_API void RemoveAll();

        PX_API TW TweenZoom(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenZoom(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenPos(const Vec2& from, const Vec2&  to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);

        PX_API TW QueueTweenZoom(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenZoom(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenPos(const Vec2& from, const Vec2&  to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenRotation(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
    private:
        PX_INTERNAL Mat4 CalculateViewMatrix();
    private:
        std::vector<DRAWABLE> m_Objects;
        WINDOW m_Wnd;
    };

    typedef Camera* CAMERA;
}