#pragma once

#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/utility/Color.h"
#include "pixl/core/animation/Animation.h"
#include "pixl/utility/Axis.h"
#include "pixl/core/tween/Tweenable.h"

#include <unordered_map>

namespace px
{
    class PX_API Sprite : public Drawable, public Tweenable
    {
    public:
        Vec2 offset;
        Vec2 pivotPoint = Vec2(0.5f, 0.5f);
        float rotation = 0.0f;
        Vec2 scrollFactor;
		TEXTURE tex;
		Color color;
		Vec2 uvPos;
		Vec2 uvSize = Vec2(1.0f, 1.0f);
    public:
        PX_API Sprite();
        PX_API Sprite(TEXTURE tex);
        PX_API Sprite(TEXTURE tex, const Vec2& pos);
        PX_API Sprite(TEXTURE tex, const Vec2& pos, const Vec2& scrollFactor);
        PX_API ~Sprite();

        PX_API void SetTexture(TEXTURE tex);
        PX_API void AddAnimation(CREFSTR name, ANIM animation, const Vec2& offset = Vec2());
        PX_API void PlayAnimation(CREFSTR name);
        PX_API void SetDefaultAnimation(CREFSTR name);
        PX_API void Center(Axis axis);
        PX_API void Center(Axis axis, const Vec2& parentSize);

        PX_API void Draw(const DrawData& data) override;
        PX_API void Update(float delta) override;

        // You could call this a Twon of functions! ;-)

        PX_API TW TweenPos(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenPos(CREFSTR id, const Vec2& from, const Vec2&  to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenSize(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenSize(CREFSTR id, const Vec2& from, const Vec2&  to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenSize(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenSize(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenScale(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenScale(CREFSTR id, const Vec2& from, const Vec2&  to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenScale(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenScale(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(CREFSTR id, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
    private:
        std::unordered_map<std::string, ANIM> m_Animations;
        std::unordered_map<std::string, Vec2> m_AnimOffsets;
        ANIM m_CurAnim;
        std::string m_CurAnimName;
        std::string m_DefAnim;
    };
}