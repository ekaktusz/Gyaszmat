#include "ParticleEmitter.h"

 ParticleEmitter::ParticleEmitter(float x, float y, float sx, float sy, float intensity) :
	m_Position({x,y}),
	m_Size({sx, sy}),
	m_Intensity(intensity)
{}

void ParticleEmitter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const Particle& particle : m_Particles)
	{
		target.draw(particle);
	}
}

void ParticleEmitter::update()
{
	Particle particle(m_Position.x, m_Position.y, rand() % (10-5 + 1) + 5, 5, rand() % (10-5 + 1) + 5, sf::Color.Red, sf::Color.Red, 1);
	m_Particles.push_back(particle);
	for (Particle& particle : m_Particles)
	{
		particle.update();
	}
}

