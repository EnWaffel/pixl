#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/asset/AssetPackage.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/core/asset/AudioBuffer.h"
#include "pixl/core/asset/Font.h"
#include "pixl/core/asset/Model.h"
#include "pixl/core/asset/TextureAtlas.h"
#include "pixl/core/asset/CubeMap.h"

#include <unordered_map>

#define PX_ERROR_ASSET_NOT_AVAILABLE 0x40
#define PX_ERROR_ASSET_LOAD_FAILED 0x41
#define PX_ERROR_ASSET_INTERNAL_ERROR 0x42
#define PX_ERROR_ASSET_INVALID_DATA 0x43
#define PX_ERROR_ASSET_LIB_ERROR 0x44

#define PX_ASTREAM_REF(s) *s.get()

namespace px
{
    class AssetManager
    {
    public:
        PX_INTERNAL AssetManager() = delete;
        PX_INTERNAL ~AssetManager() = delete;
        PX_INTERNAL static Error Init();
        PX_INTERNAL static void End();

        /**
         * @brief Adds an Asset Package to the list of available packages.
         * 
         * @param package The package to add.
         */
        PX_API static void AddPackage(APKG package);
        /**
         * @brief Sets if an asset should be searched for inside a package or system directory first.
         */
        PX_API static void SetPreferPackages(bool flag);
        /**
         * @brief Searches for an asset at the specified path inside an Asset Package or system directory and returns a stream or nullptr.
         * 
         * @return A stream for an asset.
         */
        PX_API static std::unique_ptr<std::istream> GetStream(CREFSTR path);

        /**
         * @brief Loads a texture from an Asset Package or system directory.
         * 
         * @param id An identifier for the loaded asset.
         * @param path The path to the texture.
         * @param antialiasing false for pixel art.
         * @param reload Whether to reload the cached asset.
         * @param manage Whether to cache the asset. (When set to false this asset is not released automatically)
         * 
         * @return A new texture object or nullptr.
         */
        PX_API static TEXTURE LoadTexture(CREFSTR id, CREFSTR path, bool antialiasing = true, bool reload = false, bool manage = true);
        /**
         * @brief Creates a texture from raw image data.
         * 
         * @param id An identifier for the created asset.
         * @param img The image data of the texture.
         * @param antialiasing false for pixel art.
         * @param reload Whether to reload the cached asset.
         * @param manage Whether to cache the asset. (When set to false this asset is not released automatically)
         * 
         * @return A new texture object or nullptr.
         */
        PX_API static TEXTURE LoadTexture(CREFSTR id, const ImageData& img, bool antialiasing = true, bool reload = false, bool manage = true);
        /**
         * @brief Loads an image from an Asset Package or system directory.
         * 
         * @param path The path to the image file.
         * 
         * @return The raw image data. (ImageData::data needs to be freed manually!)
         */
        PX_API static ImageData LoadImage(CREFSTR path);
        /**
         * @brief Loads a sound from an Asset Package or system directory.
         * 
         * @param id An identifier for the loaded asset.
         * @param path The path to the sound.
         * @param reload Whether to reload the cached asset.
         * 
         * @return A new sound object or nullptr.
         */
        PX_API static AUDIOBUF LoadSound(CREFSTR id, CREFSTR path, bool reload = false);
        /**
         * @brief Loads a font from an Asset Package or system directory.
         * 
         * @param id An identifier for the loaded asset.
         * @param path The path to the font.
         * @param size The size of the font in pixels.
         * @param antialiasing false for pixel art.
         * @param reload Whether to reload the cached asset.
         * 
         * @return A new font object or nullptr.
         */
        PX_API static FONT LoadFont(CREFSTR id, CREFSTR path, uint16_t size = 24, bool antialiasing = true, bool reload = false);
#ifdef PX_3D
        /**
         * @brief Loads a 3D Model from an Asset Package or system directory.
         * 
         * @param id An identifier for the loaded asset.
         * @param path The path to the 3D Model.
         * @param antialiasing false for pixel art
         * @param reload Whether to reload the cached asset.
         * 
         * @return A new model object or nullptr.
         */
        PX_API static MODEL LoadModel(CREFSTR id, CREFSTR path, bool antialiasing = true, bool reload = false);
#endif
        /**
         * @brief Loads a texture atlas from an Asset Package or system directory.
         * 
         * @param id An identifier for the loaded asset.
         * @param path The path to the atlas.
         * @param meta The metadata of the atlas. (Use the appropriate px::TextureAtlas::Load... method for your atlas type)
         * @param antialiasing false for pixel art.
         * @param reload Whether to reload the cached asset.
         * 
         * @return A new texture atlas object or nullptr.
         */
        PX_API static TEXTUREATLAS LoadAtlas(CREFSTR id, CREFSTR path, const AtlasMeta& meta, bool antialiasing = true, bool reload = false);

        /**
         * @brief Loads a cubemap from an Asset Package or system directory.
         * 
         * @param id An identifier for the loaded asset.
         * @param right The path to the right image of the cubemap.
         * @param left The path to the left image of the cubemap.
         * @param top The path to the top image of the cubemap.
         * @param bottom The path to the bottom image of the cubemap.
         * @param front The path to the front image of the cubemap.
         * @param back The path to the back image of the cubemap.
         * @param antialiasing false for pixel art.
         * @param reload Whether to reload the cached asset.
         * 
         * @return A new cubemap object or nullptr.
         */
        PX_API static CUBEMAP LoadCubeMap(CREFSTR id, CREFSTR right, CREFSTR left, CREFSTR top, CREFSTR bottom, CREFSTR front, CREFSTR back, bool antialiasing = true, bool reload = false);

        /**
         * @brief Returns a cached texture with the specified id.
         * 
         * @returns A cached texture.
         */
        PX_API static TEXTURE GetTexture(CREFSTR id);
        /**
         * @brief Returns a cached sound with the specified id.
         * 
         * @returns A cached sound.
         */
        PX_API static AUDIOBUF GetSound(CREFSTR id);
        /**
         * @brief Returns a cached font with the specified id.
         * 
         * @returns A cached font.
         */
        PX_API static FONT GetFont(CREFSTR id);
#ifdef PX_3D
        PX_API static MODEL GetModel(CREFSTR id);
#endif
        /**
         * @brief Returns a cached texture atlas with the specified id.
         * 
         * @returns A cached texture atlas.
         */
        PX_API static TEXTUREATLAS GetAtlas(CREFSTR id);
        /**
         * @brief Returns a SubTexture from a texture atlas.
         * 
         * @param id The id of the texture atlas.
         * @param name The name of the frame from the atlas.
         * 
         * @return A SubTexture containing the frame data.
         */
        PX_API static SubTexture GetSub(CREFSTR id, CREFSTR name);

        /**
         * @brief Releases all texture assets with the specified prefix.
         * 
         * @param prefix The prefix.
         */
        PX_API static void ReleaseTexturesWithPrefix(CREFSTR prefix);
        /**
         * @brief Releases all sound assets with the specified prefix.
         * 
         * @param prefix The prefix.
         */
        PX_API static void ReleaseSoundsWithPrefix(CREFSTR prefix);
        /**
         * @brief Releases all font assets with the specified prefix.
         * 
         * @param prefix The prefix.
         */
        PX_API static void ReleaseFontsWithPrefix(CREFSTR prefix);
#ifdef PX_3D
        /**
         * @brief Releases all 3d model assets with the specified prefix.
         * 
         * @param prefix The prefix.
         */
        PX_API static void ReleaseModelsWithPrefix(CREFSTR prefix);
#endif
        /**
         * @brief Releases all assets with the specified prefix.
         * 
         * @param prefix The prefix.
         */
        PX_API static void ReleaseAllWithPrefix(CREFSTR prefix);
    };
}