#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/graphics/Drawable.h"

namespace px
{
    class VideoPlayer;
    typedef VideoPlayer* VIDPLAYER;
    struct VideoPlayerInitData;

    class VideoPlayerImpl;
    class VideoPlayer : public Drawable
    {
    public:
        Vec2 pos;
        Vec2 size;
        Vec2 uvPos;
        Vec2 uvSize = Vec2(1.0f);
        Vec2 scrollFactor = Vec2(1.0f);
        Vec2 pivotPoint = Vec2(0.5f);
        Vec2 scale = Vec2(1.0f);
        bool flipX = false;
        bool flipY = false;
        float rotation = 0.0f;
    public:
        PX_INTERNAL VideoPlayer(VideoPlayerInitData& data);
        PX_INTERNAL ~VideoPlayer();

        PX_API void Play();
        PX_API void Pause();
        PX_API void Resume();
        PX_API void Stop();

        PX_API void Draw(const DrawData& data) override;
        PX_API void Update(float delta) override;

        PX_API bool IsPlaying();
        PX_API bool IsPaused();
        PX_API bool IsFinished();

        /**
         * managed: Should the video player be automatically deleted when pixl ends?
         */
        static PX_API VIDPLAYER Create(CREFSTR path, bool managed = true, uint8_t frameBufferSize = 4);
        static PX_API void Destroy(VIDPLAYER player);
    private:
        VideoPlayerImpl* m_Impl;
    };
}