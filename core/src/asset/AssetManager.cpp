#include "pixl/core/asset/AssetManager.h"

#include <vector>
#include <istream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <unordered_map>
#include <AL/al.h>
#include <AL/alc.h>
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace px;
namespace fs = std::filesystem;

static std::vector<APKG> __packages;
static bool __prefer_packages = false;
static FT_Library __ft = nullptr;

// Assets
static std::unordered_map<std::string, TEXTURE> __textures;
static std::unordered_map<std::string, AUDIOBUF> __audio_buffers;
static std::unordered_map<std::string, FONT> __fonts;
static std::unordered_map<std::string, MODEL> __models;
static std::unordered_map<std::string, TEXTUREATLAS> __atlases;
static std::unordered_map<std::string, CUBEMAP> __cubemaps;

static bool EndsWith(CREFSTR filename, CREFSTR ext) {
    if (filename.length() < ext.length()) return false;
    return filename.compare(filename.length() - ext.length(), ext.length(), ext) == 0;
}

static int AvailableInPackages(CREFSTR path)
{
    int count = 0;
    
    for (APKG package : __packages)
    {
        if (package->HasFile(path)) count++;
    }

    return count;
}

static std::unique_ptr<std::istream> GetFileStream(CREFSTR path)
{
    if (!fs::exists(path)) return nullptr;
    return std::move(std::make_unique<std::ifstream>(path, std::ios::binary));
}

static std::unique_ptr<std::istream> GetPackageStream(CREFSTR path)
{
    int count = AvailableInPackages(path);
    if (count != 1)
    {
        return nullptr;
    }

    for (APKG package : __packages)
    {
        if (package->HasFile(path))
        {
            return std::move(package->OpenStream(path));
        }
    }

    return nullptr;
}

static std::unique_ptr<std::istream> _GetStream(CREFSTR path)
{
    PX_DEBUG_LOG("_GetStream()", "Requested stream for: %s", path.c_str());
    std::unique_ptr<std::istream> stream = nullptr;
    int streamType = -1;

    if (__prefer_packages)
    {
        stream = GetPackageStream(path);
        streamType = 1;
        if (!stream)
        {
            stream = GetFileStream(path);
            streamType = 0;
        }
    }
    else
    {
        stream = GetFileStream(path);
        streamType = 0;
        if (!stream)
        {
            stream = GetPackageStream(path);
            streamType = 1;
        }
    }

    if (!stream) streamType = -1;

#ifdef PX_DEBUG
    if (streamType == 1)
    {
        PX_DEBUG_LOG("_GetStream()", "Got Package stream for: %s", path.c_str());
    }
    else if (streamType == 0)
    {
        PX_DEBUG_LOG("_GetStream()", "Got File stream for: %s", path.c_str());
    }
#endif

    return streamType != -1 ? std::move(stream) : nullptr;
}

Error px::AssetManager::Init()
{
    FT_Error error = FT_Init_FreeType(&__ft);
    if (error)
    {
        Error::Throw(PX_ERROR_ASSET_LIB_ERROR, std::string("Failed to initialize freetype (") + FT_Error_String(error) + ")");
        return PX_ERROR_ASSET_LIB_ERROR;
    }

    return PX_NOERROR;
}

void px::AssetManager::End()
{
    for (APKG package : __packages)
    {
        AssetPackage::ClosePackage(package);
    }

    for (const auto& v : __textures)
    {
        PX_DEBUG_LOG("AssetManager::End()", "Releasing texture: %s", v.first.c_str());
        delete v.second;
    }
    
    for (const auto& v : __audio_buffers)
    {
        PX_DEBUG_LOG("AssetManager::End()", "Releasing audio buffer: %s", v.first.c_str());
        delete v.second;
    }

    for (const auto& v : __fonts)
    {
        PX_DEBUG_LOG("AssetManager::End()", "Releasing font: %s", v.first.c_str());
        delete v.second;
    }

#ifdef PX_3D
    for (const auto& v : __models)
    {
        PX_DEBUG_LOG("AssetManager::End()", "Releasing model: %s", v.first.c_str());
        delete v.second;
    }
#endif

    for (const auto& v : __atlases)
    {
        PX_DEBUG_LOG("AssetManager::End()", "Releasing atlas: %s", v.first.c_str());
        delete v.second;
    }

    for (const auto& v : __cubemaps)
    {
        PX_DEBUG_LOG("AssetManager::End()", "Releasing cubemap: %s", v.first.c_str());
        delete v.second;
    }

    if (__ft) FT_Done_FreeType(__ft);
}

void px::AssetManager::AddPackage(APKG package)
{
    if (!package) return;
    __packages.push_back(package);
}

void px::AssetManager::SetPreferPackages(bool flag)
{
    __prefer_packages = flag;
}

std::unique_ptr<std::istream> px::AssetManager::GetStream(CREFSTR path)
{
    return _GetStream(path);
}

TEXTURE px::AssetManager::LoadTexture(CREFSTR id, CREFSTR path, bool antialiasing, bool reload, bool manage)
{
    if (__textures.count(id) > 0 && !reload)
    {
        return __textures.at(id);
    }
    else if (__textures.count(id) > 0 && reload)
    {
        delete __textures.at(id);
        __textures.erase(id);
    }
    
    ImageData img = LoadImage(path);
    if (!img.data) return nullptr;

    TEXTURE tex = LoadTexture(id, img, antialiasing, reload, manage);
    if (!tex)
    {
        stbi_image_free(img.data);
        return nullptr;
    }

    stbi_image_free(img.data);

    return tex;
}

TEXTURE px::AssetManager::LoadTexture(CREFSTR id, const ImageData& img, bool antialiasing, bool reload, bool manage)
{
    if (__textures.count(id) > 0 && !reload)
    {
        return __textures.at(id);
    }
    else if (__textures.count(id) > 0 && reload)
    {
        delete __textures.at(id);
        __textures.erase(id);
    }

    if (img.width < 1 || img.height < 1 || img.size < 1)
    {
        Error::Throw(PX_ERROR_ASSET_INVALID_DATA, "Image size must at least be 1x1");
        return nullptr;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glActiveTexture(GL_TEXTURE0);

    GLuint tex;
    glGenTextures(1, &tex);
    if (!tex)
    {
        Error::Throw(PX_ERROR_ASSET_INTERNAL_ERROR, "OpenGL Error (glGenTextures)");
        return nullptr;
    }

    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (antialiasing)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    GLint format = GL_INVALID_ENUM;

    switch (img.format)
    {
    case ImageFormat::RGB:
        format = GL_RGB;
        break;
    case ImageFormat::RGBA:
        format = GL_RGBA;
        break;
    default:
        Error::Throw(PX_ERROR_ASSET_INVALID_DATA, "Unknown image format: " + std::to_string((int)img.format));
        return nullptr;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, img.width, img.height, 0, format, GL_UNSIGNED_BYTE, img.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    ImageData d{};
    d.format = img.format;
    d.width = img.width;
    d.height = img.height;
    d.size = tex;

    TEXTURE texture = new Texture(d);

    if (manage) __textures.insert({ id, texture });

    return texture;
}

ImageData px::AssetManager::LoadImage(CREFSTR path)
{
    std::unique_ptr<std::istream> stream = GetStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_ASSET_NOT_AVAILABLE, "No file / stream found for: " + path);
        return { nullptr, 0, 0, 0, (ImageFormat)-1 };
    }

    stbi_io_callbacks ioCallbacks{};

    ioCallbacks.read = [](void* user, char* data, int size) -> int
    {
        std::istream* s = (std::istream*)user;
        s->read(data, size);
        return s->gcount();
    };

    ioCallbacks.skip = [](void* user, int n) -> void
    {
        std::istream* s = (std::istream*)user;
        s->seekg(n, std::ios::cur);
    };

    ioCallbacks.eof = [](void* user) -> int
    {
        std::istream* s = (std::istream*)user;
        return s->eof();
    };

    stbi_set_flip_vertically_on_load(false);

    int width, height, channels;
    stbi_uc* data = stbi_load_from_callbacks(&ioCallbacks, stream.get(), &width, &height, &channels, 4);
    if (!data)
    {
        Error::Throw(PX_ERROR_ASSET_LOAD_FAILED, "Failed to read image data from: " + path);
        return { nullptr, 0, 0, 0, (ImageFormat)-1 };
    }
    
    ImageData img{};
    img.data = data;
    img.width = width;
    img.height = height;
    img.size = width * height * 4;
    img.format = ImageFormat::RGBA;

    return img;
}

AUDIOBUF px::AssetManager::LoadSound(CREFSTR id, CREFSTR path, bool reload)
{
    if (__audio_buffers.count(id) > 0 && !reload)
    {
        return __audio_buffers.at(id);
    }
    else if (__audio_buffers.count(id) > 0 && reload)
    {
        delete __audio_buffers.at(id);
        __audio_buffers.erase(id);
    }

    std::unique_ptr<std::istream> stream = GetStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_ASSET_NOT_AVAILABLE, "No file / stream found for: " + path);
        return nullptr;
    }

    AudioData data;

    if (EndsWith(path, ".ogg"))
    {
        std::optional<AudioData> opt = AudioBuffer::DecodeOGG(*stream.get());
        if (!opt)
        {
            Error::Throw(PX_ERROR_ASSET_LOAD_FAILED, "Failed to load (ogg) audio: " + path);
            return nullptr;
        }

        data = opt.value();
    }
    else
    {
        Error::Throw(PX_ERROR_ASSET_NOT_AVAILABLE, "Unsupported audio format: " + path);
        return nullptr;
    }

    ALenum format = AL_FORMAT_STEREO16;

    if (data.channels == 1)
    {
        if (data.bitsPerSample == 8)
        {
            format = AL_FORMAT_MONO8;
        }
        else if (data.bitsPerSample == 16)
        {
            format = AL_FORMAT_MONO16;
        }
    }
    else if (data.channels == 2)
    {
        if (data.bitsPerSample == 8)
        {
            format = AL_FORMAT_STEREO8;
        }
        else if (data.bitsPerSample == 16)
        {
            format = AL_FORMAT_STEREO16;
        }
    }

    ALuint buf;
    alGenBuffers(1, &buf);

    alBufferData(buf, format, data.data, data.size, data.sampleRate);

    free(data.data);

    AudioData d = data;
    d.size = buf;

    AUDIOBUF buffer = new AudioBuffer(d);

    __audio_buffers.insert({ id, buffer });

    return buffer;
}

FONT px::AssetManager::LoadFont(CREFSTR id, CREFSTR path, uint16_t size, bool antialiasing, bool reload)
{
    if (__fonts.count(id) > 0 && !reload)
    {
        return __fonts.at(id);
    }
    else if (__fonts.count(id) > 0 && reload)
    {
        delete __fonts.at(id);
        __fonts.erase(id);
    }

    std::unique_ptr<std::istream> stream = GetStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_ASSET_NOT_AVAILABLE, "No file / stream found for: " + path);
        return nullptr;
    }

    stream->seekg(0, std::ios::end);
    uint32_t bufSize = stream->tellg();
    stream->seekg(0);
    
    uint8_t* buf = new uint8_t[bufSize];
    stream->read(reinterpret_cast<char*>(buf), bufSize);

    FT_Face face;
    FT_Error error;

    error = FT_New_Memory_Face(__ft, buf, bufSize, 0, &face);
    if (error)
    {
        delete[] buf;
        const char* err = FT_Error_String(error);
        Error::Throw(PX_ERROR_ASSET_LOAD_FAILED, std::string("Failed to load font: ") + path + " (" + std::string(err ? err : "unknown freetype error") + ":" + std::to_string(error) + ")");
        return nullptr;
    }

    FONT fnt = new Font(buf, face, size, antialiasing);

    __fonts.insert({ id, fnt });

    return fnt;
}

#ifdef PX_3D
MODEL px::AssetManager::LoadModel(CREFSTR id, CREFSTR path, bool antialiasing, bool reload)
{
    if (__models.count(id) > 0 && !reload)
    {
        return __models.at(id);
    }
    else if (__models.count(id) > 0 && reload)
    {
        delete __models.at(id);
        __models.erase(id);
    }

    std::unique_ptr<std::istream> stream = GetStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_ASSET_NOT_AVAILABLE, "No file / stream found for: " + path);
        return nullptr;
    }
    
    MODEL model = Model::Load(id, PX_ASTREAM_REF(stream), antialiasing);
    if (!model)
    {
        Error::Throw(PX_ERROR_ASSET_LOAD_FAILED, "Failed to load model: " + path);
        return nullptr;
    }

    __models.insert({ id, model });

    return model;
}
#endif

TEXTUREATLAS px::AssetManager::LoadAtlas(CREFSTR id, CREFSTR path, const AtlasMeta& meta, bool antialiasing, bool reload)
{
    if (__atlases.count(id) > 0 && !reload)
    {
        return __atlases.at(id);
    }
    else if (__atlases.count(id) > 0 && reload)
    {
        delete __atlases.at(id);
        __atlases.erase(id);
    }

    TEXTUREATLAS atlas = new TextureAtlas(LoadTexture(id, path + PX_SEPARATOR + meta.imagePath, antialiasing, reload, false), meta.subTextures);
    __atlases.insert({ id, atlas });

    return atlas;
}

CUBEMAP px::AssetManager::LoadCubeMap(CREFSTR id, CREFSTR right, CREFSTR left, CREFSTR top, CREFSTR bottom, CREFSTR front, CREFSTR back, bool antialiasing, bool reload)
{
    if (__cubemaps.count(id) > 0 && !reload)
    {
        return __cubemaps.at(id);
    }
    else if (__cubemaps.count(id) > 0 && reload)
    {
        delete __cubemaps.at(id);
        __cubemaps.erase(id);
    }

    std::string faces[6] = {
        right,
        left,
        top,
        bottom,
        front,
        back
    };

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    for (unsigned int i = 0; i < 6; i++)
    {
        ImageData data = LoadImage(faces[i]);
        if (!data.data)
        {
            Error::Throw(PX_ERROR_ASSET_LOAD_FAILED, "Failed to load cubemap face: " + faces[i]);
            glDeleteTextures(1, &tex);
            return nullptr;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data);

        stbi_image_free(data.data);
    }

    if (antialiasing)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    CUBEMAP cubemap = new CubeMap(tex);
    __cubemaps.insert({ id, cubemap });

    return cubemap;
}

TEXTURE px::AssetManager::GetTexture(CREFSTR id)
{
    if (__textures.count(id) < 1) return nullptr;
    return __textures.at(id);
}

AUDIOBUF px::AssetManager::GetSound(CREFSTR id)
{
    if (__audio_buffers.count(id) < 1) return nullptr;
    return __audio_buffers.at(id);
}

FONT px::AssetManager::GetFont(CREFSTR id)
{
    if (__fonts.count(id) < 1) return nullptr;
    return __fonts.at(id);
}

#ifdef PX_3D
MODEL px::AssetManager::GetModel(CREFSTR id)
{
    if (__models.count(id) < 1) return nullptr;
    return __models.at(id);
}
#endif

TEXTUREATLAS px::AssetManager::GetAtlas(CREFSTR id)
{
    if (__atlases.count(id) < 1) return nullptr;
    return __atlases.at(id);
}

SubTexture px::AssetManager::GetSub(CREFSTR id, CREFSTR name)
{
    if (__atlases.count(id) < 1) return {};
    return __atlases.at(id)->Get(name);
}

void px::AssetManager::ReleaseTexturesWithPrefix(CREFSTR prefix)
{
    std::vector<std::string> toRemove;

    for (auto& v : __textures)
    {
        if (v.first.size() < prefix.size() || v.first.substr(0, prefix.size()) != prefix) continue;
        toRemove.push_back(v.first);
        delete v.second;
        PX_DEBUG_LOG("AssetManager::ReleaseTexturesWithPrefix()", "Releasing texture: %s", v.first.c_str());
    }

    for (CREFSTR id : toRemove)
    {
        __textures.erase(id);
    }
}

void px::AssetManager::ReleaseSoundsWithPrefix(CREFSTR prefix)
{
    std::vector<std::string> toRemove;

    for (auto& v : __audio_buffers)
    {
        if (v.first.size() < prefix.size() || v.first.substr(0, prefix.size()) != prefix) continue;
        toRemove.push_back(v.first);
        delete v.second;
        PX_DEBUG_LOG("AssetManager::ReleaseSoundsWithPrefix()", "Releasing audio buffer: %s", v.first.c_str());
    }

    for (CREFSTR id : toRemove)
    {
        __audio_buffers.erase(id);
    }
}

void px::AssetManager::ReleaseFontsWithPrefix(CREFSTR prefix)
{
    std::vector<std::string> toRemove;

    for (auto& v : __fonts)
    {
        if (v.first.size() < prefix.size() || v.first.substr(0, prefix.size()) != prefix) continue;
        toRemove.push_back(v.first);
        delete v.second;
        PX_DEBUG_LOG("AssetManager::ReleaseFontsWithPrefix()", "Releasing font: %s", v.first.c_str());
    }

    for (CREFSTR id : toRemove)
    {
        __fonts.erase(id);
    }
}

#ifdef PX_3D
void px::AssetManager::ReleaseModelsWithPrefix(CREFSTR prefix)
{
    std::vector<std::string> toRemove;

    for (auto& v : __models)
    {
        if (v.first.size() < prefix.size() || v.first.substr(0, prefix.size()) != prefix) continue;
        toRemove.push_back(v.first);
        delete v.second;
        PX_DEBUG_LOG("AssetManager::ReleaseModelsWithPrefix()", "Releasing model: %s", v.first.c_str());
    }

    for (CREFSTR id : toRemove)
    {
        __models.erase(id);
    }
}
#endif

void px::AssetManager::ReleaseAllWithPrefix(CREFSTR prefix)
{
    ReleaseTexturesWithPrefix(prefix);
    ReleaseSoundsWithPrefix(prefix);
    ReleaseFontsWithPrefix(prefix);
#ifdef PX_3D
    ReleaseModelsWithPrefix(prefix);
#endif
}
