#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"

namespace px
{
    struct PX_API SoundGroup
    {
        std::string id;
        float volume = 1.0f;
    };

    typedef SoundGroup* SNDGROUP;
}