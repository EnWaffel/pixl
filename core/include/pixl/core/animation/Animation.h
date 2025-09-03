#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/core/asset/Texture.h"

#include <vector>

namespace px
{
    struct AnimationFrame
    {
        TEXTURE tex;
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
        /**
         * @brief Constructs an invalid Animation instance.
         */
        PX_API Animation();
        /**
         * @brief Constructs a new Animation instance.
         * 
         * @param frames A list of frames.
         * @param fps The FPS for the animation to run at.
         * @param looping Whether the animation should loop.
         */
        PX_API Animation(std::vector<AnimationFrame>& frames, uint8_t fps, bool looping);
        PX_API ~Animation();

        /**
         * @brief Play an animation.
         */
        PX_API void Play();
        /**
         * @brief Pauses an animation.
         */
        PX_API void Pause();
        /**
         * @brief Resumes a paused animation.
         */
        PX_API void Resume();
        /**
         * @brief Stops an animation.
         */
        PX_API void Stop();

        /**
         * @brief Updates an animation.
         */
        PX_API void Update(float delta);

        /**
         * @brief Returns the current frame of an animation.
         * 
         * @return A reference to the current frame.
         */
        PX_API AnimationFrame& GetCurrentFrame();
        /**
         * @return Whether the animation is playing or not.
         */
        PX_API bool IsPlaying();
        /**
         * @return Whether the animation is finished or not. (Never returns true if the animation is looping)
         */
        PX_API bool IsFinished();
        /**
         * @return Whether the animation is looping or not.
         */
        PX_API bool IsLooping();
        /**
         * @brief Sets whether an animation should loop or not.
         */
        PX_API void SetLooping(bool looping);
        /**
         * @brief Sets the FPS of an animation.
         */
        PX_API void SetFPS(uint8_t fps);
        /**
         * @return Whether the animation should reset to the first frame after playing.
         */
        PX_API bool IsResetOnFinish();
        /**
         * @brief Sets if an animation should reset to the first frame after playing.
         */
        PX_API void SetResetOnFinish(bool resetOnFinish);

        /**
         * @return Whether the animation is valid or not.
         */
        PX_API operator bool();

        /**
         * @brief Returns an animation object of all frames starting with a specified prefix from an animation map.
         * 
         * @param prefix The prefix.
         * @param frames A reference to an animation map.
         * @param looping Whether the resulting animation should loop or not.
         * 
         * @return An animation object of all frames starting with the prefix.
         */
        PX_API static ANIM GetFromPrefix(CREFSTR prefix, ANIMFRAMES& frames, bool looping = false);
    private:
        std::vector<AnimationFrame> m_Frames;
        uint8_t m_FPS;
        bool m_Looping;
        bool m_Playing;
        bool m_Paused;
        bool m_Finished;
        bool m_ResetOnFinish;
        float m_Waited;
        uint16_t m_Idx;
    };
}