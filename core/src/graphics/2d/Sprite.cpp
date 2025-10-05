#include "pixl/core/graphics/2d/Sprite.h"
#include "pixl/core/window/Window.h"

#include <glad/glad.h>

using namespace px;

extern WINDOW __pixl_rootwnd;

px::Sprite::Sprite() : subTex({ {}, {}, {}, {}, 0.0f })
{
	PX_R_DECL_FIELD(pos, Vec2);
	PX_R_DECL_FIELD(size, Vec2);
	PX_R_DECL_FIELD(scale, Vec2);
	PX_R_DECL_FIELD(offset, Vec2);
	PX_R_DECL_FIELD(pivotPoint, Vec2);
	PX_R_DECL_FIELD(scrollFactor, float);
	PX_R_DECL_FIELD(tex, TEXTURE);
	PX_R_DECL_FIELD(color, Color);
	PX_R_DECL_FIELD(uvPos, Vec2);
	PX_R_DECL_FIELD(uvSize, Vec2);
	PX_R_DECL_FIELD(flipX, bool);
	PX_R_DECL_FIELD(flipY, bool);
	PX_R_DECL_FIELD(atlas, TEXTUREATLAS);
	PX_R_DECL_FIELD(subTex, SubTexture);
	PX_R_DECL_FIELD(order, int);
	PX_R_DECL_FIELD(visible, bool);
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

void px::Sprite::SetTexture(CREFSTR name)
{
	if (!atlas) return;
	tex = atlas->GetTexture();
	subTex = atlas->Get(name);
	size = subTex.screenSize;
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

	AnimationFrame& frame = m_CurAnim.GetCurrentFrame();
	tex = frame.tex;
	uvPos = frame.uvPos;
	uvSize = frame.uvSize;
	offset = frame.offset + m_AnimOffsets.at(m_CurAnimName);
	size.x = frame.uvSize.x * tex->GetWidth();
	size.y = frame.uvSize.y * tex->GetHeight();
	rotation = frame.rotation;
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
	for (auto& v : m_Animations)
	{
		if (v.first == name) return &v.second;
	}

	return nullptr;
}

ANIM* px::Sprite::GetCurrentAnimation()
{
	return &m_CurAnim;
}

CREFSTR px::Sprite::GetCurrentAnimationName()
{
	return m_CurAnimName;
}

void px::Sprite::Draw(const DrawData& data)
{
	SHADER shd = shader != nullptr ? shader : data.shaders[PX_SHD_IDX_SPRITE];
	shd->Use();

	if (shd == shader)
	{
		shd->SetMatrix4("projection_matrix", data.projectionMatrix);
		shd->SetMatrix4("view_matrix", data.viewMatrix);
		shd->SetVec2("px_resolution", data.wnd->GetFixedSize());
	}

	Vec2 drawOffset;
	Vec2 drawUVPos = uvPos;
	Vec2 drawUVSize = uvSize;
	Vec2 texSize = tex ? Vec2(tex->GetWidth(), tex->GetHeight()) : Vec2();
	Vec2 drawSize = size;

	if (atlas)
	{
		drawUVPos = Vec2(subTex.offset) / texSize;
		drawUVSize = Vec2(subTex.size) / texSize;
		drawSize = subTex.screenSize;
	}

    Vec2 drawPos = pos;
	drawPos.x -= data.offset.x * scrollFactor.x;
	drawPos.y -= data.offset.y * scrollFactor.y;

	Mat4 mat;
	Vec2 diff = Vec2((drawSize.x * scale.x) - drawSize.x, (drawSize.y * scale.y) - drawSize.y);

	mat.Translate(Vec2(drawPos.x - (diff.x / 2.0f), drawPos.y - (diff.y / 2.0f)).asVec3());

	mat.Translate(Vec2(drawSize.x * pivotPoint.x * scale.x, drawSize.y * pivotPoint.y * scale.y).asVec3());
	mat.Rotate(Vec3(0.0f, 0.0f, rotation + subTex.screenRotation));
	mat.Translate(Vec2(-(drawSize.x * pivotPoint.x) * scale.x, -(drawSize.y * pivotPoint.y) * scale.y).asVec3());
	
	mat.Translate(offset.asVec3() + Vec3(subTex.screenOffset.x, subTex.screenOffset.y, 0));

	mat.Scale(Vec2(drawSize.x + diff.x, drawSize.y + diff.y).asVec3());

    shd->SetMatrix4("model_matrix", mat);
	shd->SetVec2("px_uv_coord", drawUVPos);
	shd->SetVec2("px_uv_size", drawUVSize);
	shd->SetBool("px_flip_x", flipX);
	shd->SetBool("px_flip_y", flipY);
	shd->SetColor("px_color", color);

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
			if (m_CurAnim.IsResetOnFinish())
			{
				tex = nullptr;
				uvPos = Vec2();
				uvSize = Vec2(1.0f);
				offset = Vec2();
				size = Vec2();
				rotation = 0.0f;
			}

			m_CurAnim = ANIM();
			m_CurAnimName.clear();

			if (!m_DefAnim.empty())
			{
				PlayAnimation(m_DefAnim);
			}

			return;
		}

		AnimationFrame& frame = m_CurAnim.GetCurrentFrame();

		tex = frame.tex;
		uvPos = frame.uvPos;
		uvSize = frame.uvSize;
		offset = frame.offset + m_AnimOffsets.at(m_CurAnimName);
		size.x = frame.uvSize.x * tex->GetWidth();
		size.y = frame.uvSize.y * tex->GetHeight();
		rotation = frame.rotation;
	}
}

TW px::Sprite::TweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::TweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(from, to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::TweenSize(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(to, &size, duration, easing, delay, callback);
}

TW px::Sprite::TweenSize(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(from, to, &size, duration, easing, delay, callback);
}

TW px::Sprite::TweenScale(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::TweenScale(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenVec2(from, to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::TweenRotation(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return TweenFloat(to, &rotation, duration, easing, delay, callback);
}

TW px::Sprite::TweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return TweenFloat(from, to, &rotation, duration, easing, delay, callback);
}

// Queued tweens

TW px::Sprite::QueueTweenPos(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::QueueTweenPos(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(from, to, &pos, duration, easing, delay, callback);
}

TW px::Sprite::QueueTweenSize(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(to, &size, duration, easing, delay, callback);
}

TW px::Sprite::QueueTweenSize(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(from, to, &size, duration, easing, delay, callback);
}

TW px::Sprite::QueueTweenScale(const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::QueueTweenScale(const Vec2& from, const Vec2& to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenVec2(from, to, &scale, duration, easing, delay, callback);
}

TW px::Sprite::QueueTweenRotation(float to, float duration, const Easing::EasingFunc &easing, float delay, const TweenCompleteCallback &callback)
{
    return QueueTweenFloat(to, &rotation, duration, easing, delay, callback);
}

TW px::Sprite::QueueTweenRotation(float from, float to, float duration, const Easing::EasingFunc& easing, float delay, const TweenCompleteCallback& callback)
{
    return QueueTweenFloat(from, to, &rotation, duration, easing, delay, callback);
}
