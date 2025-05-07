#include "pixl/core/animation/loader/SparrowAtlasLoader.h"

#include <fstream>
#include <rapidxml.hpp>
#include <sstream>
#include <iostream>

using namespace px;
using namespace rapidxml;

struct Rect
{
    float x;
    float y;
    float w;
    float h;
};

static float GetFloat(xml_node<>* node, const char* attribute)
{
    return atof(node->first_attribute(attribute)->value());
}

static bool GetBool(xml_node<>* node, const char* attribute)
{
    return std::string(node->first_attribute(attribute)->value()) == "true";
}

static bool HasAttribute(xml_node<>* node, const char* attribute)
{
    return node->first_attribute(attribute);
}

static ANIMFRAMES _Load(std::istream& stream, const ImageData& img)
{
    ANIMFRAMES frames;

    std::stringstream buf;
    buf << stream.rdbuf();

    std::string content = buf.str();
    std::vector<char> data(content.begin(), content.end());
    data.push_back(0);

    try
    {
        xml_document<> doc;
        doc.parse<0>(&data[0]);

        xml_node<>* root = doc.first_node("TextureAtlas");
        if (!root) throw std::runtime_error("No root element");

        xml_attribute<>* imgPath = root->first_attribute("imagePath");
        if (!imgPath) throw std::runtime_error("Element TextureAtlas is missing the 'imagePath' attribute");

        for (xml_node<>* node = root->first_node(); node; node = node->next_sibling())
        {
            std::string name = node->first_attribute("name")->value();
            AnimationFrame frame{};
            
            bool trimmed = HasAttribute(node, "frameX");
            bool rotated = (HasAttribute(node, "rotated") && GetBool(node, "rotated"));
            bool flipX = (HasAttribute(node, "flipX") && GetBool(node, "flipX"));
            bool flipY = (HasAttribute(node, "flipY") && GetBool(node, "flipY"));
        
            Rect rect = {
                GetFloat(node, "x"),
                GetFloat(node, "y"),
                GetFloat(node, "width"),
                GetFloat(node, "height")
            };
        
            Rect size;
            if (trimmed)
            {
                size = {
                    GetFloat(node, "frameX"),
                    GetFloat(node, "frameY"),
                    GetFloat(node, "frameWidth"),
                    GetFloat(node, "frameHeight")
                };
            }
            else
            {
                size = {
                    0.0f,
                    0.0f,
                    rect.w,
                    rect.h
                };
            }
        
            Vec2 sourceSize = Vec2(size.w, size.h);
            if (rotated && !trimmed) sourceSize = Vec2(size.h, size.w);
        
            frame.uvPos = Vec2(rect.x / img.width, rect.y / img.height);
            frame.uvSize = Vec2(rect.w / img.width, rect.h / img.height);
            frame.size = sourceSize;
            frame.rotation = rotated ? -90.0f : 0.0f;
            frame.offset = Vec2(-size.x, -size.y);

            frames.push_back(std::make_pair(name, frame));
        }
    }
    catch (const parse_error& e)
    {
        Error::Throw(PX_ERROR_INVALID_OPERATION, std::string("Failed to parse XML: ") + e.what());
    }
    catch (const std::exception& e)
    {
        Error::Throw(PX_ERROR_INVALID_OPERATION, std::string("Failed to parse XML: ") + e.what());
    }
    
    return std::move(frames);
}

/*
ANIMFRAMES px::SparrowAtlasLoader::Load(APKG package, CREFSTR path, const ImageData& img)
{
    if (!package)
    {
        Error::Throw(PX_ERROR_INVALID_OPERATION, "Package is nullptr");
        return ANIMFRAMES();
    }

    if (!package->HasFile(path))
    {
        Error::Throw(PX_ERROR_INVALID_OPERATION, "File not found in package: " + path);
        return ANIMFRAMES();
    }

    auto stream = package->OpenStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_INVALID_OPERATION, "Failed to open package stream for: " + path);
        return ANIMFRAMES();
    }

    return std::move(_Load(*stream.get(), img));
}
*/

ANIMFRAMES px::SparrowAtlasLoader::Load(CREFSTR path, const ImageData& img)
{
    std::ifstream stream(path);
    if (!stream.is_open())
    {
        Error::Throw(PX_ERROR_INVALID_OPERATION, "Can't open file: " + path);
        return ANIMFRAMES();
    }

    return std::move(_Load(stream, img));
}
