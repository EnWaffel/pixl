#include "pixl/core/sound/Sound.h"
#include "pixl/core/sound/SoundSystem.h"

#include <AL/al.h>

using namespace px;

extern float __pixl_global_volume;

px::Sound::Sound(AUDIOBUF buffer, SNDGROUP group, bool temp) : m_Buffer(buffer), m_Volume(1.0f), m_Temp(temp)
{
    m_Group = group;

    alGenSources(1, &m_Data);
    alSourcei(m_Data, AL_BUFFER, buffer->m_Data.size);
    if (temp)
    {
        SoundSystem::UpdateVolume();
    }
    else
    {
        SetVolume(m_Volume);
    }
}

px::Sound::~Sound()
{
    alDeleteSources(1, &m_Data);
}

void px::Sound::Play()
{
    alSourcePlay(m_Data);
}

void px::Sound::Pause()
{
    alSourcePause(m_Data);
}

void px::Sound::Resume()
{
    alSourcePlay(m_Data);
}

void px::Sound::Stop()
{
    alSourceStop(m_Data);
}

void px::Sound::SetVolume(float volume)
{
    m_Volume = volume;
    if (m_Temp)
    {
        SoundSystem::UpdateVolume();
    }
    else
    {
        alSourcef(m_Data, AL_GAIN, m_Volume * m_Group->volume * __pixl_global_volume);
    }
}

void px::Sound::SetPitch(float pitch)
{
    alSourcef(m_Data, AL_PITCH, pitch);
}

float px::Sound::GetTimePosition()
{
    ALint samples = 0;
    alGetSourcei(m_Data, AL_SAMPLE_OFFSET, &samples);
    return ((float)samples * 1000.0f) / m_Buffer->m_Data.sampleRate;
}

bool px::Sound::IsPlaying()
{
    ALint state;
    alGetSourcei(m_Data, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

void px::Sound::SetPosition(const Vec3& pos)
{
    alSourcefv(m_Data, AL_POSITION, (ALfloat*)&pos);
}

void px::Sound::SetVelocity(const Vec3& vel)
{
    alSourcefv(m_Data, AL_VELOCITY, (ALfloat*)&vel);
}

void px::Sound::SetDirection(const Vec3& direction)
{
    alSourcefv(m_Data, AL_DIRECTION, (ALfloat*)&direction);
}

void px::Sound::SetCone(const DirectionCone& cone)
{
    alSourcef(m_Data, AL_CONE_INNER_ANGLE, cone.innerAngle);
    alSourcef(m_Data, AL_CONE_OUTER_ANGLE, cone.outerAngle);
    alSourcef(m_Data, AL_CONE_OUTER_GAIN, cone.outerGain);
}
