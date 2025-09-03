#include "pixl/core/graphics/3d/Camera3D.h"
#include "pixl/core/window/Window.h"
#include "pixl/core/pixl.h"

#include <algorithm>
#include <iostream>
#include <math.h>

using namespace px;

px::Camera3D::Camera3D(WINDOW window) : m_Wnd(window), m_Front(Vec3(0.0f, 0.0f, -1.0f)), m_WorldUp(Vec3(0.0f, 1.0f, 0.0f)), m_Up(Vec3(0.0f, 1.0f, 0.0f))
{
    CalculateViewMatrix();
}

void Camera3D::Update(float delta)
{
    UpdateTweens(delta);

    for (DRAWABLE drawable : m_Objects)
    {
        drawable->Update(delta);
    }
}

void Camera3D::Draw(DRAWINGCTX ctx, SHADER objectShader, SHADER skyboxShader)
{
    DrawData data{};
    Mat4 viewMatrix = CalculateViewMatrix();

    data.wnd = m_Wnd;
    data.ctx = ctx;

    data.projectionMatrix = Mat4::Perspective(fov, m_Wnd->GetFixedSize().x / m_Wnd->GetFixedSize().y, 0.1f, 10000.0f);
    data.viewMatrix = viewMatrix;
    data.viewMatrix.Translate(offset);

    data.shaders[PX_SHD_IDX_OBJECT] = objectShader;
    data.shaders[PX_SHD_IDX_SKYBOX] = skyboxShader;

    objectShader->Use();
    objectShader->SetMatrix4("projection_matrix", data.projectionMatrix);
    objectShader->SetMatrix4("view_matrix", viewMatrix);

    Mat4 skyboxMatrix = viewMatrix;
    skyboxMatrix(3, 0) = 0.0f;
    skyboxMatrix(3, 1) = 0.0f;
    skyboxMatrix(3, 2) = 0.0f;

    skyboxShader->Use();
    skyboxShader->SetMatrix4("projection_matrix", data.projectionMatrix);
    skyboxShader->SetMatrix4("view_matrix", skyboxMatrix);

    if (skybox)
    {
        skybox->Draw(data);
    }

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

void px::Camera3D::RemoveAll()
{
    m_Objects.clear();
}

TW px::Camera3D::TweenFOV(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::TweenFOV(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(from, to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::TweenPos(const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec3(to, &pos, duration, easing, delay, callback);
}

TW px::Camera3D::TweenPos(const Vec3& from, const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec3(from, to, &pos, duration, easing, delay, callback);
}

// Queued tweens

TW px::Camera3D::QueueTweenFOV(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return QueueTweenFloat(to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::QueueTweenFOV(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenFloat(from, to, &fov, duration, easing, delay, callback);
}

TW px::Camera3D::QueueTweenPos(const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec3(to, &pos, duration, easing, delay, callback);
}

TW px::Camera3D::QueueTweenPos(const Vec3& from, const Vec3& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec3(from, to, &pos, duration, easing, delay, callback);
}

Mat4 px::Camera3D::CalculateViewMatrix()
{
    float actualYaw = yaw + yawOffset;
    float actualPitch = pitch + pitchOffset;
    float actualRoll = roll + rollOffset;

    Vec3 front;
    front.x = cos(Math::ToRadians(actualYaw)) * cos(Math::ToRadians(actualPitch));
    front.y = sin(Math::ToRadians(actualPitch));
    front.z = sin(Math::ToRadians(actualYaw)) * cos(Math::ToRadians(actualPitch));
    m_Front = front.Normalize();

    m_Right = m_Front.Cross(m_WorldUp).Normalize();
    m_Up = m_Right.Cross(m_Front).Normalize();

    if (actualRoll != 0.0f)
    {
        float cosRoll = cos(Math::ToRadians(actualRoll));
        float sinRoll = sin(Math::ToRadians(actualRoll));

        Vec3 rotatedRight = m_Right * cosRoll - m_Up * sinRoll;
        Vec3 rotatedUp = m_Right * sinRoll + m_Up * cosRoll;

        m_Right = rotatedRight.Normalize();
        m_Up = rotatedUp.Normalize();
    }

    return Mat4::LookAt(pos, pos + m_Front, m_Up);
}
