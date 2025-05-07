#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/asset/AssetPackage.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/core/asset/AudioBuffer.h"

#define PX_ERROR_ASSET_NOT_AVAILABLE 0x40
#define PX_ERROR_ASSET_LOAD_FAILED 0x41
#define PX_ERROR_ASSET_INTERNAL_ERROR 0x42
#define PX_ERROR_ASSET_INVALID_DATA 0x43

namespace px
{
    class AssetManager
    {
    public:
        PX_INTERNAL AssetManager() = delete;
        PX_INTERNAL ~AssetManager() = delete;
        PX_INTERNAL static Error Init();
        PX_INTERNAL static void End();

        PX_API static void AddPackage(APKG package);
        PX_API static void SetPreferPackages(bool flag);

        PX_API static TEXTURE LoadTexture(CREFSTR id, CREFSTR path, bool antialiasing = true, bool reload = false);
        PX_API static TEXTURE LoadTexture(CREFSTR id, const ImageData& img, bool antialiasing = true, bool reload = false);
        PX_API static AUDIOBUF LoadSound(CREFSTR id, CREFSTR path, bool reload = false);

        PX_API static TEXTURE GetTexture(CREFSTR id);
        PX_API static AUDIOBUF GetSound(CREFSTR id);
    };
}