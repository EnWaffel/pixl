#pragma once

#include "pixl/core/graphics/Drawable.h"
#include "pixl/core/graphics/2d/Sprite.h"

#include <vector>

namespace px
{
    class SpriteGroup : public Drawable
    {
    public:
        PX_API SpriteGroup();
        PX_API ~SpriteGroup();

        PX_API void Add(Sprite* sprite);
        PX_API void Add(Sprite& sprite);
        PX_API void Remove(Sprite* sprite);
        PX_API void Remove(Sprite& sprite);

        PX_API Vec2 GetPosition();
        PX_API void SetPosition(const Vec2& pos);
        PX_API Vec2 GetSize();

        PX_API void Draw(const DrawData& data) override;
        PX_API void Update(float delta) override;
    private:
        std::vector<Sprite*> m_Children;
    };
}