#pragma once

#include "pixl/utility/Defs.h"

#include <optional>
#include <istream>

namespace px
{
    class Sound;

    struct PX_API AudioData
    {
        int16_t* data;
        uint32_t size;
        uint32_t sampleRate;
        uint8_t channels;
        uint8_t bitsPerSample;
    };

    class AudioBuffer
    {
    public:
        PX_INTERNAL AudioBuffer(const AudioData& data);
        PX_INTERNAL ~AudioBuffer();

        PX_API static std::optional<AudioData> DecodeOGG(std::istream& stream);
    protected:
        AudioData m_Data;

    friend class Sound;
    };

    typedef AudioBuffer* AUDIOBUF;
}