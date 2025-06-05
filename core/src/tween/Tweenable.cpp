#include "pixl/core/tween/Tweenable.h"

#include <algorithm>

using namespace px;

px::Tweenable::~Tweenable()
{
    CancelAllTweens();
}

TW px::Tweenable::TweenFloat(float from, float to, float* ptr, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<float>(from, to, ptr, duration, easing, delay, callback);

    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenFloat(float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<float>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec2(const Vec2& from, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec2>(from, to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec2(const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec2>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec3(const Vec3& from, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec3>(from, to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenVec3(const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec3>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenColor(const Color& from, const Color &to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Color>(from, to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

TW px::Tweenable::TweenColor(const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Color>(to, ptr, duration, easing, delay, callback);
    
    if (!StartTween(t))
    {
        delete t;
        return nullptr;
    }

    return t;
}

// Queued tweens

TW px::Tweenable::QueueTweenFloat(float from, float to, float* ptr, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<float>(from, to, ptr, duration, easing, delay, callback);

    QueueTween(t);

    return t;
}

TW px::Tweenable::QueueTweenFloat(float to, float* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<float>(to, ptr, duration, easing, delay, callback);
    
    QueueTween(t);

    return t;
}

TW px::Tweenable::QueueTweenVec2(const Vec2& from, const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec2>(from, to, ptr, duration, easing, delay, callback);
    
    QueueTween(t);

    return t;
}

TW px::Tweenable::QueueTweenVec2(const Vec2& to, Vec2* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec2>(to, ptr, duration, easing, delay, callback);
    
    QueueTween(t);

    return t;
}

TW px::Tweenable::QueueTweenVec3(const Vec3& from, const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec3>(from, to, ptr, duration, easing, delay, callback);
    
    QueueTween(t);

    return t;
}

TW px::Tweenable::QueueTweenVec3(const Vec3& to, Vec3* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Vec3>(to, ptr, duration, easing, delay, callback);
    
    QueueTween(t);

    return t;
}

TW px::Tweenable::QueueTweenColor(const Color& from, const Color &to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Color>(from, to, ptr, duration, easing, delay, callback);
    
    QueueTween(t);

    return t;
}

TW px::Tweenable::QueueTweenColor(const Color& to, Color* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    TW t = new Tween<Color>(to, ptr, duration, easing, delay, callback);
    
    QueueTween(t);

    return t;
}

bool px::Tweenable::StartTween(TW tween)
{
    if (std::find(m_Tweens.begin(), m_Tweens.end(), tween) != m_Tweens.end()) return false;
    m_Tweens.push_back(tween);
    return true;
}

void px::Tweenable::QueueTween(TW tween)
{
    m_Queue.push(tween);
}

void px::Tweenable::CancelTween(TW tween)
{
    auto it = std::find(m_Tweens.begin(), m_Tweens.end(), tween);
    if (it == m_Tweens.end()) return;
    m_Tweens.erase(it);
    delete tween;
}

void px::Tweenable::UpdateTweens(float delta)
{
    if (m_CurTween)
    {
        m_CurTween->Update(delta);

        if (m_CurTween->IsFinished() || m_CurTween->IsCanceled())
        {
            CancelCurrentTween();
        }
    }
    else
    {
        if (!m_Queue.empty())
        {
            m_CurTween = m_Queue.front();
            m_Queue.pop();
        }
    }

    std::vector<TweenBase*> toCancel;

    for (TweenBase* tw : m_Tweens)
    {
        tw->Update(delta);

        if (tw->IsFinished() || tw->IsCanceled())
        {
            toCancel.push_back(tw);
        }
    }
    
    for (TweenBase* tw : toCancel)
    {
        CancelTween(tw);
    }
}

void px::Tweenable::CancelTweens()
{
    for (TweenBase* tw : m_Tweens)
    {
        delete tw;
    }

    m_Tweens.clear();
}

void px::Tweenable::CancelQueuedTweens()
{
    while (!m_Queue.empty())
    {
        TweenBase* tw = m_Queue.front();
        m_Queue.pop();
        delete tw;
    }
}

void px::Tweenable::CancelCurrentTween()
{
    if (!m_CurTween) return;
    delete m_CurTween;
    m_CurTween = nullptr;
}

void px::Tweenable::CancelAllTweens()
{
    CancelTweens();
    CancelQueuedTweens();
    CancelCurrentTween();
}
