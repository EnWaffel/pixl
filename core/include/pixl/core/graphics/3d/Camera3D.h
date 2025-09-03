#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/graphics/Shader.h"
#include "pixl/core/tween/Tweenable.h"
#include "pixl/core/graphics/3d/Skybox.h"

#include <vector>

namespace px
{
    class Window;
    typedef Window* WINDOW;

    class Camera3D : public Tweenable
    {
    public:
        Vec3 pos;
        Vec3 offset;
        float yaw = 0.0f;
        float pitch = 0.0f;
        float roll = 0.0f;
        float fov = 45.0f;
        float yawOffset = 0.0f;
        float pitchOffset = 0.0f;
        float rollOffset = 0.0f;
        Skybox* skybox = nullptr;
    public:
        PX_API Camera3D(WINDOW window);

        PX_API void Update(float delta);
        PX_API void Draw(DRAWINGCTX ctx, SHADER objectShader, SHADER skyboxShader);
        PX_API void Add(DRAWABLE drawable);
        PX_API void Add(Drawable& drawable);
        PX_API void Remove(DRAWABLE drawable);
        PX_API void Remove(Drawable& drawable);
        PX_API void Reorder();
        PX_API void RemoveAll();

        PX_API TW TweenFOV(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenFOV(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenPos(const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenPos(const Vec3& from, const Vec3&  to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);

        PX_API TW QueueTweenFOV(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenFOV(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenPos(const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenPos(const Vec3& from, const Vec3&  to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
    private:
        PX_INTERNAL Mat4 CalculateViewMatrix();
    private:
        std::vector<DRAWABLE> m_Objects;
        WINDOW m_Wnd;
    public:
        Vec3 m_Front;
        Vec3 m_Up;
        Vec3 m_Right;
        Vec3 m_WorldUp;
    };

    typedef Camera3D* CAMERA3D;
}