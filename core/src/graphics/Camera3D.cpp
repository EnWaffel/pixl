#include "pixl/core/graphics/Camera3D.h"
#include "pixl/core/window/Window.h"
#include "pixl/core/pixl.h"

#include <algorithm>
#include <iostream>
#include <math.h>

using namespace px;

px::Camera3D::Camera3D(WINDOW window) : m_Wnd(window), m_Front(Vec3(0.0f, 0.0f, -1.0f)), m_Up(Vec3(0.0f, 1.0f, 0.0f))
{
}

void Camera3D::Update(float delta)
{
    UpdateTweens(delta);

    for (DRAWABLE drawable : m_Objects)
    {
        drawable->Update(delta);
    }
}

void Camera3D::Draw(DRAWINGCTX ctx, SHADER objectShader)
{
    DrawData data{};
    data.ctx = ctx;
    data.shd = objectShader;

    data.shd->Use();
    data.shd->SetMatrix4("view_matrix", CalculateViewMatrix());

    for (DRAWABLE drawable : m_Objects)
    {
        if (!drawable->visible) continue;
        drawable->Draw(data);
    }
}

void px::Camera3D::Add(DRAWABLE drawable)
{
    m_Objects.push_back(drawable);
}

void px::Camera3D::Add(Drawable& drawable)
{
    m_Objects.push_back(&drawable);
}

void px::Camera3D::Remove(DRAWABLE drawable)
{
    auto it = std::find(m_Objects.begin(), m_Objects.end(), drawable);
    if (it == m_Objects.end()) return;
    m_Objects.erase(it);
}

void px::Camera3D::Remove(Drawable& drawable)
{
    Remove(&drawable);
}

void px::Camera3D::Reorder()
{
    std::vector<Drawable*> sorted;
    std::vector<Drawable*> sorted1;
    for (Drawable* d : m_Objects)
    {
        if (d->order < 0)
        {
            sorted1.push_back(d);
        }
        else
        {
            sorted.push_back(d);
        }
    }

    std::sort(sorted.begin(), sorted.end(), [](Drawable* d1, Drawable* d2) { return d1->order > d2->order; });

    for (Drawable* d : sorted) sorted1.push_back(d);
    m_Objects = sorted1;
}

TW px::Camera3D::TweenFOV(CREFSTR id, float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(id, to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::TweenFOV(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(id, from, to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::TweenFOV(float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("fov");
    UpdateTweens(0.0f);
    return TweenFloat("fov", to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::TweenFOV(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("fov");
    UpdateTweens(0.0f);
    return TweenFloat("fov", from, to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::TweenPos(CREFSTR id, const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec3(id, to, &pos, duration, easing, delay, callback);
}

TW px::Camera3D::TweenPos(CREFSTR id, const Vec3& from, const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec3(id, from, to, &pos, duration, easing, delay, callback);
}

TW px::Camera3D::TweenPos(const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("pos");
    UpdateTweens(0.0f);
    return TweenVec3("pos", to, &pos, duration, easing, delay, callback);
}

TW px::Camera3D::TweenPos(const Vec3& from, const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("pos");
    UpdateTweens(0.0f);
    return TweenVec3("pos", from, to, &pos, duration, easing, delay, callback);
}

Mat4 px::Camera3D::CalculateViewMatrix()
{
    Vec3 front;
    front.x = cos(Math::ToRadians(yaw)) * cos(Math::ToRadians(pitch));
    front.y = sin(Math::ToRadians(pitch));
    front.z = sin(Math::ToRadians(yaw)) * cos(Math::ToRadians(pitch));
    front = front.Normalize();

    m_Right = front.Cross(m_WorldUp).Normalize();
    m_Up = m_Right.Cross(m_Front).Normalize();

    return Mat4::LookAt(pos, pos + m_Front, m_Up);
}
