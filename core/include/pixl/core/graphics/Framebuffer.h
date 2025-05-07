#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2i.h"

namespace px
{
    class Framebuffer
    {
    public:
        PX_API Framebuffer(const Vec2i& size, bool transparent);
        PX_API ~Framebuffer();

        PX_API void Bind();
        PX_API void Unbind();
        PX_API void BindForDrawing();
    private:
        unsigned int m_Handle[2];
    };

    typedef Framebuffer* FRAMEBUF;
}