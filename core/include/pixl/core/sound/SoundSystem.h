#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/sound/Sound.h"
#include "pixl/core/sound/SoundGroup.h"

#define PX_ERROR_SND_DEVICE 0x90
#define PX_ERROR_SND_INTERNAL 0x91

namespace px
{
    class SoundSystem
    {
    public:
        PX_INTERNAL SoundSystem() = delete;
        PX_INTERNAL ~SoundSystem() = delete;
        PX_INTERNAL static Error Init();
        PX_INTERNAL static void End();
        PX_INTERNAL static void Update(float delta);

        PX_API static void PlaySound(AUDIOBUF buffer, float volume = 1.0f, CREFSTR group = "default");
        PX_API static void PlaySound(CREFSTR id, float volume = 1.0f, CREFSTR group = "default");
        PX_API static SOUND CreateSound(AUDIOBUF buffer, float volume = 1.0f, CREFSTR group = "default");
        PX_API static SOUND CreateSound(CREFSTR id, float volume = 1.0f, CREFSTR group = "default");

        PX_API static void CreateGroup(CREFSTR id, float volume);
        PX_API static void DeleteGroup(CREFSTR id);
        PX_API static SNDGROUP GetGroup(CREFSTR id);

        PX_API static void UpdateVolume();
        PX_API static void SetGlobalVolume(float volume);
    public:
        static SNDGROUP DEFAULT_GROUP;
    };
}