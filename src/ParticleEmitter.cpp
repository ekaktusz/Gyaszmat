#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(float x, float y, float sx, float sy, float intensity, ParticleEmitter::Type type) :
	m_Position({ x, y }),
	m_Size({ x, y }),
	m_Intensity(intensity),
	m_Type(type)
{}

void ParticleEmitter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (Particle* particle : m_Particles)
	{
		target.draw(*particle);
	}
}

void ParticleEmitter::update()
{
	Particle* particle;
}

Particle* ParticleEmitter::generateParticle()
{
	return nullptr;
}




