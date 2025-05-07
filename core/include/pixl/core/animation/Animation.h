#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"

#include <vector>

namespace px
{
    struct AnimationFrame
    {
        Vec2 offset;
        Vec2 size;
        Vec2 uvPos;
        Vec2 uvSize;
        float rotation;
    };

    typedef std::vector<std::pair<std::string, AnimationFrame>> ANIMFRAMES;

    class Animation;
    typedef Animation ANIM;

    class Animation
    {
    public:
        PX_API Animation();
        PX_API Animation(std::vector<AnimationFrame>& frames, uint8_t fps, bool looping);
        PX_API ~Animation();

        PX_API void Play();
        PX_API void Pause();
        PX_API void Resume();
        PX_API void Stop();

        PX_API void Update(float delta);

        PX_API AnimationFrame& GetCurrentFrame();
        PX_API bool IsPlaying();
        PX_API bool IsFinished();

        PX_API operator bool();

        PX_API static ANIM GetFromPrefix(CREFSTR prefix, ANIMFRAMES& frames, bool looping = false);
    private:
        std::vector<AnimationFrame> m_Frames;
        uint8_t m_FPS;
        bool m_Looping;
        bool m_Playing;
        bool m_Paused;
        bool m_Finished;
        float m_Waited;
        uint16_t m_Idx;
    };
}