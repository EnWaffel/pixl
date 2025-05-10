#include "pixl/core/sound/SoundSystem.h"
#include "pixl/core/asset/AssetManager.h"

#ifndef AL_LIBTYPE_STATIC
#define AL_LIBTYPE_STATIC
#endif
#include <AL/al.h>
#include <AL/alc.h>
#include <unordered_map>
#include <vector>

using namespace px;

static ALCcontext* __context;
static ALCdevice* __device;
static std::unordered_map<std::string, SNDGROUP> __groups;
static std::vector<SOUND> __temp_sounds;
float __pixl_global_volume = 1.0f;

SNDGROUP px::SoundSystem::DEFAULT_GROUP = nullptr;

Error px::SoundSystem::Init()
{
    __device = alcOpenDevice(NULL);
    if (!__device)
    {
        const ALCchar* error = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

        Error::Throw(PX_ERROR_SND_DEVICE, "Failed to open default device");
        return PX_ERROR_SND_DEVICE;
    }

    __context = alcCreateContext(__device, NULL);
    if (!__context || !alcMakeContextCurrent(__context))
    {
        Error::Throw(PX_ERROR_SND_INTERNAL, "OpenAL Error (Failed to create context)");
        return PX_ERROR_SND_INTERNAL;
    }

    CreateGroup("default", 1.0f);
    DEFAULT_GROUP = GetGroup("default");

    return PX_NOERROR;
}

void px::SoundSystem::End()
{
    for (const auto& v : __groups)
    {
        delete v.second;
    }

    for (SOUND snd : __temp_sounds)
    {
        snd->Stop();
        delete snd;
    }

    if (__context)
    {
        alcMakeContextCurrent(NULL);
        alcDestroyContext(__context);
    }
    
    if (__device)
    {
        alcCloseDevice(__device);
    }
}

void px::SoundSystem::Update(float delta)
{
    for (SOUND snd : __temp_sounds)
    {
        
    }
}

void px::SoundSystem::PlaySound(AUDIOBUF buffer, float volume, CREFSTR group)
{
    if (!buffer) return;

    SNDGROUP _group = GetGroup(group);
    if (!_group) _group = DEFAULT_GROUP;

    SOUND snd = new Sound(buffer, _group, true);
    __temp_sounds.push_back(snd);

    snd->SetVolume(volume);
    snd->Play();
}

void px::SoundSystem::PlaySound(CREFSTR id, float volume, CREFSTR group)
{
    PlaySound(AssetManager::GetSound(id), volume, group);
}

SOUND px::SoundSystem::CreateSound(AUDIOBUF buffer, float volume, CREFSTR group)
{
    return new Sound(buffer, GetGroup(group), false);
}

SOUND px::SoundSystem::CreateSound(CREFSTR id, float volume, CREFSTR group)
{
    return new Sound(AssetManager::GetSound(id), GetGroup(group), false);
}

void px::SoundSystem::CreateGroup(CREFSTR id, float volume)
{
    if (__groups.count(id) > 0) return;

    SNDGROUP group = new SoundGroup;
    group->id = id;
    group->volume = volume;

    __groups.insert({ id, group });
}

void px::SoundSystem::DeleteGroup(CREFSTR id)
{
    if (__groups.count(id) < 1) return;
    delete __groups.at(id);
    __groups.erase(id);
}

SNDGROUP px::SoundSystem::GetGroup(CREFSTR id)
{
    if (__groups.count(id) < 1) return nullptr;
    return __groups.at(id);
}

void px::SoundSystem::UpdateVolume()
{
    for (SOUND snd : __temp_sounds)
    {
        alSourcef(snd->m_Data, AL_GAIN, snd->m_Volume * snd->m_Group->volume * __pixl_global_volume);
    }
}

void px::SoundSystem::SetGlobalVolume(float volume)
{
    __pixl_global_volume = volume;
    UpdateVolume();
}
