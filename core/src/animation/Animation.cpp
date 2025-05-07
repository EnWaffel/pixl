#include "pixl/core/animation/Animation.h"

#include <iostream>

using namespace px;

px::Animation::Animation() : m_FPS(0), m_Looping(false), m_Playing(false), m_Paused(false), m_Finished(false), m_Waited(0.0f), m_Idx(0)
{
}

px::Animation::Animation(std::vector<AnimationFrame>& frames, uint8_t fps, bool looping) : m_Frames(frames), m_FPS(fps), m_Looping(looping), m_Playing(false), m_Paused(false), m_Finished(false), m_Waited(0.0f), m_Idx(0)
{
}

px::Animation::~Animation()
{
}

void px::Animation::Play()
{
    m_Playing = true;
    m_Paused = false;
    m_Finished = false;
    m_Idx = 0;
    m_Waited = 0.0f;    
}

void px::Animation::Pause()
{
    m_Paused = true;
}

void px::Animation::Resume()
{
    m_Paused = false;
    m_Playing = true;
}

void px::Animation::Stop()
{
    m_Playing = false;
}

void px::Animation::Update(float delta)
{
    if (m_Paused || !m_Playing) return;

    m_Waited += delta;
    if (m_Waited >= 1000.0f / m_FPS)
    {
        m_Waited = 0.0f;
        m_Idx++;

        if (m_Idx >= m_Frames.size())
        {
            m_Idx = 0;

            if (!m_Looping)
            {
                m_Playing = false;
                m_Finished = true;
            }
        }
    }
}

AnimationFrame& px::Animation::GetCurrentFrame()
{
    return m_Frames.at(m_Idx);
}

bool px::Animation::IsPlaying()
{
    return m_Playing && !m_Paused;
}

bool px::Animation::IsFinished()
{
    return m_Finished;
}

px::Animation::operator bool()
{
    return m_Frames.size() > 0;
}

ANIM px::Animation::GetFromPrefix(CREFSTR prefix, ANIMFRAMES &frames, bool looping)
{
    std::vector<AnimationFrame> _frames;

    for (const auto& v : frames)
    {
        if (v.first.size() >= prefix.size() && v.first.compare(0, prefix.size(), prefix) == 0)
        {
            _frames.push_back(v.second);
        }
    }

    return Animation(_frames, 24, looping);
}
