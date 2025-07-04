#include "pixl/core/asset/TextureAtlas.h"
#include "pixl/core/asset/AssetManager.h"

#include "pixl/json.hpp"
#include <fstream>

using namespace nlohmann;
using namespace px;

px::TextureAtlas::TextureAtlas(TEXTURE texture, const std::unordered_map<std::string, SubTexture>& subTextures) : m_Texture(texture), m_SubTextures(subTextures)
{
}

px::TextureAtlas::~TextureAtlas()
{
    delete m_Texture;
}

SubTexture px::TextureAtlas::Get(CREFSTR name)
{
    if (m_SubTextures.count(name) < 1) return {};
    return m_SubTextures.at(name);
}

TEXTURE px::TextureAtlas::GetTexture()
{
    return m_Texture;
}

AtlasMeta px::TextureAtlas::LoadJSONHash(CREFSTR path)
{
    std::unordered_map<std::string, SubTexture> data;
    std::string imagePath;
    float imageScale = 1.0f;
    
    auto stream = AssetManager::GetStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_ATLAS_NOT_FOUND, "Failed to get stream for: " + path);
        return {};
    }

    try
    {
        json j = json::parse(PX_ASTREAM_REF(stream));
        json frames = j["frames"];
        json meta = j["meta"];
        
        imagePath = meta["image"];
        imageScale = atof(((std::string)meta["scale"]).c_str());

        for (auto& [name, frameData] : frames.items())
        {
            SubTexture sub{};
            
            bool trimmed = frameData["trimmed"];
            bool rotated = frameData["rotated"];
            Rect frame = {
                frameData["frame"]["x"],
                frameData["frame"]["y"],
                frameData["frame"]["w"],
                frameData["frame"]["h"]
            };
            Vec2i sourceSize = {
                frameData["sourceSize"]["w"],
                frameData["sourceSize"]["h"]
            };

            sub.offset = Vec2i(frame.x, frame.y);
            
            if (rotated && !trimmed)
            {
                sub.size = Vec2i(frame.h, frame.w);
            }
            else
            {
                sub.size = Vec2i(frame.w, frame.h);
            }

            Vec2i screenSize = sourceSize;
            Vec2i screenOffset;

            if (trimmed)
            {
                Rect spriteSourceSize = {
                    frameData["spriteSourceSize"]["x"],
                    frameData["spriteSourceSize"]["y"],
                    frameData["spriteSourceSize"]["w"],
                    frameData["spriteSourceSize"]["h"]
                };

                screenOffset = Vec2i(spriteSourceSize.x, spriteSourceSize.y);
            }

            sub.screenOffset = screenOffset;
            sub.screenSize = screenSize;
            sub.screenRotation = rotated ? -90.0f : 0.0f;

            data.insert({ name, sub });
        }
    }
    catch (json::parse_error& e)
    {
        Error::Throw(PX_ERROR_ATLAS_PARSE_ERROR, std::string("Failed to parse TexturePacker atlas: ") + e.what());
    }
    catch (json::type_error& e)
    {
        Error::Throw(PX_ERROR_ATLAS_PARSE_ERROR, std::string("Failed to load TexturePacker atlas: ") + e.what());
    }
    catch (json::other_error& e)
    {
        Error::Throw(PX_ERROR_ATLAS_PARSE_ERROR, std::string("Failed to load exturePacker atlas: ") + e.what());
    }

    return { imagePath, data, imageScale };
}
