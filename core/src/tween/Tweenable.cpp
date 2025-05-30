#include "pixl/core/tween/Tweenable.h"

using namespace px;

px::Tweenable::~Tweenable()
{
    for (const auto& v : m_Tweens)
    {
        delete v.second;
    }
}

TW px::Tweenable::TweenFloat(CREFSTR id, float from, float to, float* ptr, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<float>(from, to, ptr, duration, easing, delay, callback);

    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenFloat(CREFSTR id, float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<float>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec2(CREFSTR id, const Vec2& from, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec2>(from, to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec2(CREFSTR id, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec2>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec3(CREFSTR id, const Vec3& from, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec3>(from, to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec3(CREFSTR id, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec3>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenColor(CREFSTR id, const Color& from, const Color &to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Color>(from, to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenColor(CREFSTR id, const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Color>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(id, t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

bool px::Tweenable::StartTween(CREFSTR id, TW tween)
{
    if (m_Tweens.count(id) > 0) return false;
    m_Queue.push(std::make_pair(id, tween));
    return true;
}

void px::Tweenable::CancelTween(CREFSTR id)
{
    if (m_Tweens.count(id) < 1) return;
    m_CancelQueue.push(id);
}

void px::Tweenable::UpdateTweens(float delta)
{
    if (m_DoCancelAll)
    {
        m_DoCancelAll = false;

        for (const auto& v : m_Tweens)
        {
            delete v.second;
        }

        m_Tweens.clear();
    }

    while (!m_CancelQueue.empty())
    {
        std::string& id = m_CancelQueue.front();

        if (m_Tweens.count(id) > 0)
        {
            delete m_Tweens.at(id);
            m_Tweens.erase(id);
        }

        m_CancelQueue.pop();
    }

    while (!m_Queue.empty())
    {
        auto& v = m_Queue.front();

        m_Tweens.insert(v);
        
        m_Queue.pop();
    }

    for (const auto& v : m_Tweens)
    {
        v.second->Update(delta);
        if (v.second->IsFinished())
        {
            CancelTween(v.first);
        }
    }
}

void px::Tweenable::CancelAllTweens()
{
    m_DoCancelAll = true;
}
