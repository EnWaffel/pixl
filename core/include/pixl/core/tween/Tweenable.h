#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/tween/Tween.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Vec3.h"
#include "pixl/utility/Color.h"

#include <vector>
#include <queue>

namespace px
{
    class PX_API Tweenable
    {
    public:
        PX_API virtual ~Tweenable();

        PX_API TW TweenFloat(float from, float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenFloat(float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec2(const Vec2& from, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec2(const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec3(const Vec3& from, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenVec3(const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenColor(const Color& from, const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenColor(const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);

        PX_API TW QueueTweenFloat(float from, float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenFloat(float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenVec2(const Vec2& from, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenVec2(const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenVec3(const Vec3& from, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenVec3(const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenColor(const Color& from, const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW QueueTweenColor(const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        
        PX_API bool StartTween(TW tween);
        PX_API void QueueTween(TW tween);
        PX_API void CancelTween(TW tween);
        PX_API void UpdateTweens(float delta);
        PX_API void CancelTweens();
        PX_API void CancelQueuedTweens();
        PX_API void CancelCurrentTween();
        PX_API void CancelAllTweens();
    protected:
        std::vector<TweenBase*> m_Tweens;
        TweenBase* m_CurTween = nullptr;
        std::queue<TweenBase*> m_Queue;
    };
}