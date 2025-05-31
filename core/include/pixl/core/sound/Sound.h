#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/asset/AudioBuffer.h"
#include "pixl/core/sound/SoundGroup.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Vec3.h"

namespace px
{
    class SoundSystem;

    struct DirectionCone
    {
        float innerAngle;
        float outerAngle;
        float outerGain;
    };

    class Sound
    {
    public:
        PX_INTERNAL Sound(AUDIOBUF buffer, SNDGROUP group, bool temp);
        PX_INTERNAL ~Sound();

        PX_API void Play();
        PX_API void Pause();
        PX_API void Resume();
        PX_API void Stop();
        PX_API void SetVolume(float volume);
        PX_API void SetPitch(float pitch);
        PX_API float GetTimePosition();
        PX_API bool IsPlaying();

        PX_API void SetPosition(const Vec3& pos);
        PX_API void SetVelocity(const Vec3& vel);
        PX_API void SetDirection(const Vec3& direction);
        PX_API void SetCone(const DirectionCone& cone);
    private:
        unsigned int m_Data;
        AUDIOBUF m_Buffer;
        float m_Volume;
        SNDGROUP m_Group;
        bool m_Temp;
    
    friend class SoundSystem;
    };

    typedef Sound* SOUND;
}