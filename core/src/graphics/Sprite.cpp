#include "pixl/core/graphics/Sprite.h"
#include "pixl/core/window/Window.h"

#include <glad/glad.h>

using namespace px;

extern WINDOW __pixl_rootwnd;

px::Sprite::Sprite()
{
}

px::Sprite::Sprite(TEXTURE tex) : Sprite()
{
	SetTexture(tex);
}

px::Sprite::Sprite(TEXTURE tex, const Vec2& pos) : Sprite()
{
	this->pos = pos;

	SetTexture(tex);
}

px::Sprite::Sprite(TEXTURE tex, const Vec2& pos, const Vec2& scrollFactor) : Sprite()
{
	this->pos = pos;
	this->scrollFactor = scrollFactor;

	SetTexture(tex);
}

px::Sprite::~Sprite()
{
}

void px::Sprite::SetTexture(TEXTURE tex)
{
	if (!tex) return;

	this->tex = tex;
	size = Vec2(tex->GetWidth(), tex->GetHeight());
}

void px::Sprite::AddAnimation(CREFSTR name, ANIM animation, const Vec2& offset)
{
	if (!animation) return;
	m_Animations.insert({ name, animation });
	m_AnimOffsets.insert({ name, offset });
}

void px::Sprite::PlayAnimation(CREFSTR name)
{
	if (m_Animations.count(name) < 1) return;
	m_CurAnim = m_Animations.at(name);
	m_CurAnimName = name;
	m_CurAnim.Play();
	m_CurAnim.Update(0.0f);
}

void px::Sprite::SetDefaultAnimation(CREFSTR name)
{
	m_DefAnim = name;
}

void px::Sprite::Center(Axis axis)
{
	if (!__pixl_rootwnd) return;
	Center(axis, __pixl_rootwnd->GetFixedSize());
}

void px::Sprite::Center(Axis axis, const Vec2& parentSize)
{
	Update(0.0f);

	if (axis == X || axis == XY)
	{
		pos.x = parentSize.x / 2.0f - size.x / 2.0f;
	}
	if (axis == Y || axis == XY)
	{
		pos.y = parentSize.y / 2.0f - size.y / 2.0f;
	}
}

ANIM* px::Sprite::GetAnimation(CREFSTR name)
{
    return &m_CurAnim;
}

void px::Sprite::Draw(const DrawData& data)
{
    data.shd->Use();

    Vec2 _pos = pos;

	Mat4 mat;
	Vec2 diff = Vec2((size.x * scale.x) - size.x, (size.y * scale.y) - size.y);

	mat.Translate(Vec2(_pos.x - (diff.x / 2.0f), _pos.y - (diff.y / 2.0f)));

	mat.Translate(Vec2(size.x * pivotPoint.x * scale.x, size.y * pivotPoint.y * scale.y));
	mat.Rotate(Vec3(0.0f, 0.0f, rotation));
	mat.Translate(Vec2(-(size.x * pivotPoint.x) * scale.x, -(size.y * pivotPoint.y) * scale.y));
	mat.Translate(offset);

	mat.Scale(Vec2(size.x + diff.x, size.y + diff.y));

    data.shd->SetMatrix4("model_matrix", mat);
	data.shd->SetVec2("px_uv_coord", uvPos);
	data.shd->SetVec2("px_uv_size", uvSize);
	data.shd->SetBool("px_flip_x", flipX);
	data.shd->SetBool("px_flip_y", flipY);
	data.shd->SetColor("px_color", color);

	if (tex)
	{
		tex->Bind();
	}

    data.ctx->DrawQuad();

	if (tex)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void px::Sprite::Update(float delta)
{
	UpdateTweens(delta);

    if (m_CurAnim && tex)
	{
		m_CurAnim.Update(delta);
		if (m_CurAnim.IsFinished())
		{
			m_CurAnim = ANIM();
			m_CurAnimName.clear();
			if (!m_DefAnim.empty())
			{
				PlayAnimation(m_DefAnim);
			}
			return;
		}

		AnimationFrame& frame = m_CurAnim.GetCurrentFrame();

		uvPos = frame.uvPos;
		uvSize = frame.uvSize;
		offset = frame.offset + m_AnimOffsets.at(m_CurAnimName);
		size.x = frame.uvSize.x * tex->GetWidth();
		size.y = frame.uvSize.y * tex->GetHeight();
		rotation = frame.rotation;
	}
}

TW px::Sprite::TweenPos(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::TweenPos(CREFSTR id, const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, from, to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::TweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("pos");
    UpdateTweens(0.0f);
    return TweenVec2("pos", to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::TweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("pos");
    UpdateTweens(0.0f);
    return TweenVec2("pos", from, to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::TweenSize(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, to, &size, duration, easing, delay, callback);
}

TW px::Sprite::TweenSize(CREFSTR id, const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, from, to, &size, duration, easing, delay, callback);
}

TW px::Sprite::TweenSize(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("size");
    UpdateTweens(0.0f);
    return TweenVec2("size", to, &size, duration, easing, delay, callback);
}

TW px::Sprite::TweenSize(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("size");
    UpdateTweens(0.0f);
    return TweenVec2("size", from, to, &size, duration, easing, delay, callback);
}

TW px::Sprite::TweenScale(CREFSTR id, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::TweenScale(CREFSTR id, const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(id, from, to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::TweenScale(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("scale");
    UpdateTweens(0.0f);
    return TweenVec2("scale", to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::TweenScale(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("scale");
    UpdateTweens(0.0f);
    return TweenVec2("scale", from, to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::TweenRotation(CREFSTR id, float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(id, to, &rotation, duration, easing, delay, callback);
}

TW px::Sprite::TweenRotation(CREFSTR id, float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(id, from, to, &rotation, duration, easing, delay, callback);
}

TW px::Sprite::TweenRotation(float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("rotation");
    UpdateTweens(0.0f);
    return TweenFloat("rotation", to, &rotation, duration, easing, delay, callback);
}

TW px::Sprite::TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    CancelTween("rotation");
    UpdateTweens(0.0f);
    return TweenFloat("rotation", from, to, &rotation, duration, easing, delay, callback);
}
