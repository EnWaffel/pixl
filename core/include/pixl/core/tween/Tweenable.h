#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/tween/Tween.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/utility/Color.h"

#include <unordered_map>
#include <queue>

namespace px
{
    class PX_API Tweenable
    {
    public:
        PX_API virtual ~Tweenable();

        PX_API TW TweenFloat(CREFSTR id, float from, float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenFloat(CREFSTR id, float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec2(CREFSTR id, const Vec2& from, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec2(CREFSTR id, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec3(CREFSTR id, const Vec3& from, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec3(CREFSTR id, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenColor(CREFSTR id, const Color& from, const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenColor(CREFSTR id, const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API bool StartTween(CREFSTR id, TW tween);
        PX_API void CancelTween(CREFSTR id);
        PX_API void UpdateTweens(float delta);
        PX_API void CancelAll();
    protected:
        std::unordered_map<std::string, TweenBase*> m_Tweens;
        std::queue<std::string> m_CancelQueue;
        std::queue<std::pair<std::string, TweenBase*>> m_Queue;
        bool m_DoCancelAll = false;
    };
}