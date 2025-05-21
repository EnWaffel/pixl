#pragma once

#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/asset/Font.h"
#include "pixl/utility/Color.h"
#include "pixl/utility/Axis.h"
#include "pixl/core/tween/Tweenable.h"

#include <unordered_map>

namespace px
{
    class PX_API Text : public Drawable, public Tweenable
    {
    public:
        Vec2 pos;
        Vec2 offset;
        Vec2 size;
        Vec2 pivotPoint = Vec2(0.5f, 0.5f);
        float rotation = 0.0f;
        Vec2 scrollFactor;
		FONT fnt;
		Color color;
        float scale = 1.0f;
        float lineSpacing = 3.0f;
        bool flipX = false;
        bool flipY = false;
    public:
        PX_API Text();
        PX_API Text(FONT fnt, const UTFString& text);
        PX_API Text(FONT fnt, const UTFString& text, const Vec2& pos);
        PX_API Text(FONT fnt, const UTFString& text, const Vec2& pos, const Vec2& scrollFactor);
        PX_API ~Text();

        PX_API void SetText(const UTFString& text);
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
        PX_API TW TweenScale(CREFSTR id, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenScale(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenScale(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenScale(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(CREFSTR id, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
        PX_API TW TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay = 0.0f, const TweenCompleteCallback& callback = nullptr);
    private:
        UTFString m_Text;
    };
}