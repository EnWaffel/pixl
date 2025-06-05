#include "pixl/core/graphics/SpriteGroup.h"

#include <algorithm>

using namespace px;

px::SpriteGroup::SpriteGroup()
{
}

px::SpriteGroup::~SpriteGroup()
{
}

void px::SpriteGroup::Add(Sprite* sprite)
{
    m_Children.push_back(sprite);
}

void px::SpriteGroup::Add(Sprite& sprite)
{
    Add(&sprite);
}

void px::SpriteGroup::Remove(Sprite* sprite)
{
    auto it = std::find(m_Children.begin(), m_Children.end(), sprite);
    if (it == m_Children.end()) return;
    m_Children.erase(it);
}

void px::SpriteGroup::Remove(Sprite& sprite)
{
    Remove(&sprite);
}

Vec2 px::SpriteGroup::GetPosition()
{
    if (m_Children.empty()) return Vec2(0, 0);

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();

    for (Sprite* child : m_Children) {
        minX = std::min(minX, child->pos.x);
        minY = std::min(minY, child->pos.y);
    }

    return Vec2(minX, minY);
}

void px::SpriteGroup::SetPosition(const Vec2& pos)
{
    Vec2 oldPos = GetPosition();
    Vec2 delta(pos.x - oldPos.x, pos.y - oldPos.y);

    for (Sprite* child : m_Children) {
        child->pos.x += delta.x;
        child->pos.y += delta.y;
    }
}

Vec2 px::SpriteGroup::GetSize()
{
    if (m_Children.empty()) return Vec2(0, 0);

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (Sprite* child : m_Children) {
        minX = std::min(minX, child->pos.x);
        minY = std::min(minY, child->pos.y);
        maxX = std::max(maxX, child->pos.x + child->size.x * child->scale.x);
        maxY = std::max(maxY, child->pos.y + child->size.y * child->scale.y);
    }

    return Vec2(maxX - minX, maxY - minY);
}

void px::SpriteGroup::Draw(const DrawData &data)
{
    for (Sprite* child : m_Children)
    {
        child->Draw(data);
    }
}

void px::SpriteGroup::Update(float delta)
{
    for (Sprite* child : m_Children)
    {
        child->Update(delta);
    }
}
