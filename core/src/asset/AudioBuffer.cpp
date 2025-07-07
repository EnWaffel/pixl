#include "pixl/core/asset/AudioBuffer.h"
#include "pixl/utility/Error.h"

#include <AL/al.h>
#include <stb_vorbis.c>
#include <iostream>

using namespace px;

px::AudioBuffer::AudioBuffer(const AudioData& data) : m_Data(data)
{
}

px::AudioBuffer::~AudioBuffer()
{
    alDeleteBuffers(1, (ALuint*)&m_Data.data);
}

std::optional<AudioData> px::AudioBuffer::DecodeOGG(std::istream& stream)
{
    stream.seekg(0, std::ios::end);
    uint32_t size = stream.tellg();
    stream.seekg(0);
    
    uint8_t* buf = new uint8_t[size];
    stream.read(reinterpret_cast<char*>(buf), size);
    
    short* output;
    int channels, sampleRate;
    int samples = stb_vorbis_decode_memory(buf, size, &channels, &sampleRate, &output);
    
    if (samples < 0)
    {
        delete[] buf;
        Error::Throw(PX_ERROR_INVALID_OPERATION, "Failed to decode OGG stream");
        return std::nullopt;
    }
    
    delete[] buf;

    AudioData data{};
    data.channels = channels;
    data.sampleRate = sampleRate;
    data.size = samples * channels * sizeof(short);
    data.data = output;
    data.bitsPerSample = sizeof(short) * 8;

    return data;
}
