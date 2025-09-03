#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/tween/Easing.h"

#include <functional>
#include <math.h>

namespace px
{
    typedef std::function<void()> TweenCompleteCallback;

    class TweenBase
    {
    public:
        virtual ~TweenBase() = default;

        virtual void Update(float delta) = 0;
        virtual bool IsFinished() = 0;
        virtual bool IsCanceled() = 0;
        virtual void Cancel() = 0;
    };
    
    template<typename T>
    class Tween : public TweenBase
    {
    public:
        Tween(T from, T to, T* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& onComplete)
        :
        m_From(from),
        m_To(to),
        m_Ptr(ptr),
        m_Duration(duration),
        m_Easing(easing),
        m_Delay(delay),
        m_DelayElapsed(0.0f),
        m_Current(from),
        m_Canceled(false),
        m_Elapsed(0.0f),
        m_Progress(0.0f),
        m_OnComplete(onComplete) 
        {
        }

        Tween(T to, T* ptr, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& onComplete)
        :
        m_From(*ptr),
        m_To(to),
        m_Ptr(ptr),
        m_Duration(duration),
        m_Easing(easing),
        m_Delay(delay),
        m_DelayElapsed(0.0f),
        m_Current(*ptr),
        m_Canceled(false),
        m_Elapsed(0.0f),
        m_Progress(0.0f),
        m_OnComplete(onComplete)
        {
        }

        void Update(float delta) override
        {
            if (m_Canceled || m_Progress >= 1.0f) return;

            if (m_DelayElapsed < m_Delay)
			{
				m_DelayElapsed += delta;
				return;
			}

			m_Elapsed += delta;
			m_Progress = std::fminf(m_Elapsed / m_Duration, 1.0f);
			m_Current = m_From + (m_To - m_From) * m_Easing(m_Progress);
			*m_Ptr = m_Current;

            if (m_Progress >= 1.0f)
            {
                if (m_OnComplete)
                {
                    m_OnComplete();
                }
            }
        }

        bool IsFinished() override
        {
            return m_Progress >= 1.0f || m_Canceled;
        }

        bool IsCanceled() override
        {
            return m_Canceled;
        }

        void Cancel() override
        {
            m_Canceled = true;
        }
    private:
        T m_From;
        T m_To;
        T* m_Ptr;
        float m_Duration;
        Easing::EasingFunc m_Easing;
        float m_Delay;
        float m_DelayElapsed;
        T m_Current;
        bool m_Canceled;
        float m_Elapsed;
        float m_Progress;
        TweenCompleteCallback m_OnComplete;
    };

    typedef TweenBase* TW;
}