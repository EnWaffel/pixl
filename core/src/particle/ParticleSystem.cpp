#include "pixl/core/particle/ParticleSystem.h"

using namespace px;

px::ParticleSystem::ParticleSystem(WINDOW wnd)
{
}

px::ParticleSystem::~ParticleSystem()
{
}

ParticleType *px::ParticleSystem::CreateType(ParticleId id)
{
    return nullptr;
}

ParticleType* px::ParticleSystem::GetType(ParticleId id)
{
    return nullptr;
}

void px::ParticleSystem::RemoveType(ParticleId id)
{
}

void px::ParticleSystem::AddModifier(ParticleId id, ParticleModifier* modifier)
{
}

void px::ParticleSystem::RemoveModifier(ParticleId id, ParticleModifier* modifier)
{
}

void px::ParticleSystem::SpawnParticles(ParticleId id, uint16_t amount, const Vec2 &pos)
{
}
