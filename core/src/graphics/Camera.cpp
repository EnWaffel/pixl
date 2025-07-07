#include "pixl/core/graphics/Camera.h"
#include "pixl/core/window/Window.h"
#include "pixl/core/pixl.h"

#include <algorithm>
#include <iostream>

using namespace px;

px::Camera::Camera(WINDOW window) : m_Wnd(window)
{
}

void Camera::Update(float delta)
{
    if (!active) return;

    UpdateTweens(delta);

    for (DRAWABLE drawable : m_Objects)
    {
        drawable->Update(delta);
    }
}

void Camera::Draw(DRAWINGCTX ctx, SHADER spriteShader, SHADER textShader)
{
    if (!visible) return;

    DrawData data{};
    data.ctx = ctx;
    data.projectionMatrix = Mat4::Ortho(0.0f, m_Wnd->GetFixedSize().x, m_Wnd->GetFixedSize().y, 0.0f);
    data.shd = spriteShader;
    data.shd1 = textShader;
    data.offset = pos;
    data.scale = zoom;
    data.wnd = m_Wnd;

    Mat4 viewMatrix = CalculateViewMatrix();

    data.viewMatrix = viewMatrix;

    data.shd->Use();
    data.shd->SetMatrix4("view_matrix", viewMatrix);

    data.shd1->Use();
    data.shd1->SetMatrix4("view_matrix", viewMatrix);

    for (DRAWABLE drawable : m_Objects)
    {
        if (!drawable->visible) continue;
        drawable->Draw(data);
    }
}

void px::Camera::Add(DRAWABLE drawable)
{
    m_Objects.push_back(drawable);
}

void px::Camera::Add(Drawable& drawable)
{
    m_Objects.push_back(&drawable);
}

void px::Camera::Remove(DRAWABLE drawable)
{
    auto it = std::find(m_Objects.begin(), m_Objects.end(), drawable);
    if (it == m_Objects.end()) return;
    m_Objects.erase(it);
}

void px::Camera::Remove(Drawable& drawable)
{
    Remove(&drawable);
}

void px::Camera::Reorder()
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

TW px::Camera::TweenZoom(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(to, &zoom, duration, easing, delay, callback);
}

TW px::Camera::TweenZoom(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(from, to, &zoom, duration, easing, delay, callback);
}

TW px::Camera::TweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(to, &pos, duration, easing, delay, callback);
}

TW px::Camera::TweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(from, to, &pos, duration, easing, delay, callback);
}

TW px::Camera::TweenRotation(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(to, &rotation, duration, easing, delay, callback);
}

TW px::Camera::TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(from, to, &rotation, duration, easing, delay, callback);
}

// Queued tweens

TW px::Camera::QueueTweenZoom(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return QueueTweenFloat(to, &zoom, duration, easing, delay, callback);
}

TW px::Camera::QueueTweenZoom(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenFloat(from, to, &zoom, duration, easing, delay, callback);
}

TW px::Camera::QueueTweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(to, &pos, duration, easing, delay, callback);
}

TW px::Camera::QueueTweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(from, to, &pos, duration, easing, delay, callback);
}

TW px::Camera::QueueTweenRotation(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return QueueTweenFloat(to, &rotation, duration, easing, delay, callback);
}

TW px::Camera::QueueTweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenFloat(from, to, &rotation, duration, easing, delay, callback);
}

Mat4 px::Camera::CalculateViewMatrix()
{
    Mat4 mat;
    Vec2i fixedSize = m_Wnd->GetFixedSize();

    Vec2 diff = Vec2((fixedSize.x * zoom) - fixedSize.x, (fixedSize.y * zoom) - fixedSize.y);

    mat.Translate(Vec2(fixedSize.x / 2.0f * zoom, fixedSize.y / 2.0f * zoom));
    mat.Rotate(Vec3(0.0f, 0.0f, rotation));
    mat.Translate(Vec2(-(fixedSize.x / 2.0f) * zoom, -(fixedSize.y / 2.0f) * zoom));

    mat.Translate(Vec2(-diff.x / 2.0f, -diff.y / 2.0f));
    mat.Scale(Vec2(zoom, zoom));

    return mat;
}
