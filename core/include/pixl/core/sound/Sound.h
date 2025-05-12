#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/asset/AudioBuffer.h"
#include "pixl/core/sound/SoundGroup.h"

namespace px
{
    class SoundSystem;

    class Sound
    {
    public:
        PX_INTERNAL Sound(AUDIOBUF buffer, SNDGROUP group, bool temp);
        PX_API ~Sound();

        PX_API void Play();
        PX_API void Pause();
        PX_API void Resume();
        PX_API void Stop();
        PX_API void SetVolume(float volume);
        PX_API float GetTimePosition();
    private:
        unsigned int m_Data;
        AUDIOBUF m_Buffer;
        float m_Volume;
        SNDGROUP m_Group{};
        bool m_Temp;
    
    friend class SoundSystem;
    };

    typedef Sound* SOUND;
}