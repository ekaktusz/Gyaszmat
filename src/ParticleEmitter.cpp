#include "ParticleEmitter.h"

 ParticleEmitter::ParticleEmitter(float x, float y, float sx, float sy, float intensity) :
	m_Position({x,y}),
	m_Size({sx, sy}),
	m_Intensity(intensity)
{}

void ParticleEmitter::draw(RenderTarget& target, RenderStates states) const
{
	for (const Particle& particle : m_Particles)
	{
		target.draw(particle);
	}
}

void ParticleEmitter::update()
{
	for (Particle& particle : m_Particles)
	{
		particle.update();
	}
}

