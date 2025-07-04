#pragma once

#include "pixl/utility/Defs.h"
#include "pixl/utility/Error.h"
#include "pixl/core/math/Vec2.h"
#include "pixl/utility/Color.h"
#include "pixl/core/asset/Texture.h"
#include "pixl/core/window/Window.h"
#include "pixl/core/math/Mat4.h"
#include "pixl/core/graphics/Shader.h"

#include <vector>
#include <unordered_map>

namespace px
{
    typedef uint16_t ParticleId;

    struct Particle
    {
        Mat4 matrix; // (optional) to add more transformations to the particle
        Vec2 pos;
        Vec2 size;
        Vec2 vel;
        Color color;
        TEXTURE tex;
        SHADER shd;
        Vec2 uvPos;
        Vec2 uvSize;
        float lifetime;
        float maxLifetime;
    };

    class ParticleModifier
    {
    public:
        PX_API virtual ~ParticleModifier() = default;

        PX_API virtual void Transform(const std::vector<Particle>& particles) = 0;
    };

    struct ParticleType
    {
        ParticleId id;
        std::vector<Particle> particles;
        std::vector<std::pair<std::string, ParticleModifier*>> modifiers;
    };

    class ParticleSystem
    {
    public:
        PX_INTERNAL ParticleSystem(WINDOW wnd);
        PX_INTERNAL ~ParticleSystem();

        PX_API static ParticleType* CreateType(ParticleId id);
        PX_API static ParticleType* GetType(ParticleId id);
        PX_API static void RemoveType(ParticleId id);
        PX_API static void AddModifier(ParticleId id, ParticleModifier* modifier);
        PX_API static void RemoveModifier(ParticleId id, ParticleModifier* modifier);

        PX_API static void SpawnParticles(ParticleId id, uint16_t amount, const Vec2& pos);
    private:
        std::unordered_map<ParticleId, ParticleType*> m_Types;
    };
}
