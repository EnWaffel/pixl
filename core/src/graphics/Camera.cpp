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
    UpdateTweens(delta);

    for (DRAWABLE drawable : m_Objects)
    {
        drawable->Update(delta);
    }
}

void Camera::Draw(DRAWINGCTX ctx, SHADER spriteShader)
{
    DrawData data{};
    data.ctx = ctx;
    data.shd = spriteShader;
    data.shd->Use();
    spriteShader->SetMatrix4("view_matrix", CalculateViewMatrix());

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

TW px::Camera::TweenZoom(CREFSTR id, float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(id, to, &zoom, duration, easing, delay, callback);
}

TW px::Camera::TweenZoom(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(id, from, to, &zoom, duration, easing, delay, callback);
}

TW px::Camera::TweenZoom(float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("zoom");
    UpdateTweens(0.0f);
    return TweenFloat("zoom", to, &zoom, duration, easing, delay, callback);
}

TW px::Camera::TweenZoom(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("zoom");
    UpdateTweens(0.0f);
    return TweenFloat("zoom", from, to, &zoom, duration, easing, delay, callback);
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
