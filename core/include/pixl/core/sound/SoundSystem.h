#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/sound/Sound.h"
#include "pixl/core/sound/SoundGroup.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/math/Vec3.h"

#define PX_ERROR_SND_DEVICE 0x90
#define PX_ERROR_SND_INTERNAL 0x91

namespace px
{
#pragma pack(push, 1)
    struct ListenerOrientation
    {
        float forwardX;
        float forwardY;
        float forwardZ;
        float upX;
        float upY;
        float upZ;

        ListenerOrientation() : forwardX(0.0f), forwardY(0.0f), forwardZ(0.0f), upX(0.0f), upY(0.0f), upZ(0.0f) {}
        ListenerOrientation(const Vec3& forward, const Vec3& up) : forwardX(forward.x), forwardY(forward.y), forwardZ(forward.z), upX(up.x), upY(up.y), upZ(up.z) {}
    };
#pragma pack(pop)

    class SoundSystem
    {
    public:
        PX_INTERNAL SoundSystem() = delete;
        PX_INTERNAL ~SoundSystem() = delete;
        PX_INTERNAL static Error Init();
        PX_INTERNAL static void End();
        PX_INTERNAL static void Update(float delta);

        PX_API static void PlaySound(AUDIOBUF buffer, float volume = 1.0f, float pitch = 1.0f, CREFSTR group = "default");
        PX_API static void PlaySound(CREFSTR id, float volume = 1.0f, float pitch = 1.0f, CREFSTR group = "default");
        PX_API static SOUND CreateSound(AUDIOBUF buffer, float volume = 1.0f, CREFSTR group = "default");
        PX_API static SOUND CreateSound(CREFSTR id, float volume = 1.0f, CREFSTR group = "default");
        PX_API static void DestroySound(SOUND snd);

        PX_API static void CreateGroup(CREFSTR id, float volume);
        PX_API static void DeleteGroup(CREFSTR id);
        PX_API static SNDGROUP GetGroup(CREFSTR id);

        PX_API static void UpdateVolume();
        PX_API static void SetGlobalVolume(float volume);

        PX_API static void SetListenerPosition(const Vec3& pos);
        PX_API static void SetListenerPosition(const Vec2& pos);
        PX_API static void SetListenerVelocity(const Vec3& vel);
        PX_API static void SetListenerVelocity(const Vec2& vel);
        PX_API static void SetListenerOrientation(const ListenerOrientation& orientation);
    public:
        static SNDGROUP DEFAULT_GROUP;
    };
}