#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/animation/Animation.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/core/asset/AssetPackage.h"

namespace px
{
    class SparrowAtlasLoader
    {
    public:
        PX_INTERNAL SparrowAtlasLoader() = delete;
        PX_INTERNAL ~SparrowAtlasLoader() = delete;

        PX_API static ANIMFRAMES Load(CREFSTR path, const ImageData& img);
        PX_API static ANIMFRAMES Load(CREFSTR path, TEXTURE tex);
    };
}