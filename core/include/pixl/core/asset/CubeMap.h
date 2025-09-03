#pragma once

#include "pixl/utility/Defs.h"

namespace px
{
    class CubeMap
    {
    public:
        PX_INTERNAL CubeMap(unsigned int data);
        PX_API ~CubeMap();

        PX_API void Bind();
    private:
        unsigned int m_Data;
    };

    typedef CubeMap* CUBEMAP;
}