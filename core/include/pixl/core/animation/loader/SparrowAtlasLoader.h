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

        /**
         * @brief Loads all frames from a Sparrow Atlas into a map.
         * 
         * @param path The path to the xml file.
         * @param tex The texture for the atlas.
         * 
         * @return A map containing the data of all frames in the atlas.
         */
        PX_API static ANIMFRAMES Load(CREFSTR path, TEXTURE tex);
    };
}