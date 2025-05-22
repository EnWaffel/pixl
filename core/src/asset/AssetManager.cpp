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
    return std::make_unique<std::ifstream>(path, std::ios::binary);
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
            return package->OpenStream(path);
        }
    }

    return nullptr;
}

static std::unique_ptr<std::istream> _GetStream(CREFSTR path)
{
    PX_DEBUG_LOG("_GetStream()", "Requested stream for: %s", path.c_str());
    std::unique_ptr<std::istream> stream = nullptr;
#ifdef PX_DEBUG
    bool isPackageStream = false;
#endif

    if (__prefer_packages)
    {
        stream = GetPackageStream(path);
#ifdef PX_DEBUG
        isPackageStream = true;
#endif
        if (!stream)
        {
            stream = GetFileStream(path);
#ifdef PX_DEBUG
            isPackageStream = false;
#endif
        }
    }
    else
    {
        stream = GetFileStream(path);
#ifdef PX_DEBUG
        isPackageStream = false;
#endif
        if (!stream)
        {
            stream = GetPackageStream(path);
#ifdef PX_DEBUG
            isPackageStream = true;
#endif
        }
    }

#ifdef PX_DEBUG
    if (isPackageStream)
    {
        PX_DEBUG_LOG("_GetStream()", "Got Package stream for: %s", path.c_str());
    }
    else
    {
        PX_DEBUG_LOG("_GetStream()", "Got File stream for: %s", path.c_str());
    }
#endif

    return stream;
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

    for (const auto& v : __models)
    {
        PX_DEBUG_LOG("AssetManager::End()", "Releasing model: %s", v.first.c_str());
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
    return std::move(_GetStream(path));
}

TEXTURE px::AssetManager::LoadTexture(CREFSTR id, CREFSTR path, bool antialiasing, bool reload)
{
    std::unique_ptr<std::istream> stream = GetStream(path);
    if (!stream)
    {
        Error::Throw(PX_ERROR_ASSET_NOT_AVAILABLE, "No file / stream found for: " + path);
        return nullptr;
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
        return nullptr;
    }
    
    ImageData img{};
    img.data = data;
    img.width = width;
    img.height = height;
    img.size = width * height * 4;
    img.format = ImageFormat::RGBA;

    TEXTURE tex = LoadTexture(id, img, antialiasing, reload);
    if (!tex)
    {
        stbi_image_free(data);
        return nullptr;
    }

    stbi_image_free(data);

    return tex;
}

TEXTURE px::AssetManager::LoadTexture(CREFSTR id, const ImageData& img, bool antialiasing, bool reload)
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

    __textures.insert({ id, texture });

    return texture;
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
    
    MODEL model = Model::Load(id, PX_ASTREAM_REF(stream));
    if (!model)
    {
        Error::Throw(PX_ERROR_ASSET_LOAD_FAILED, "Failed to load model: " + path);
        return nullptr;
    }

    __models.insert({ id, model });

    return model;
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

MODEL px::AssetManager::GetModel(CREFSTR id)
{
    if (__models.count(id) < 1) return nullptr;
    return __models.at(id);
}
