#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(Type type) : ParticleEmitter(0, 0, 0, 0, 0, type)
{}

ParticleEmitter::ParticleEmitter(float x, float y, float sx, float sy, float intensity, ParticleEmitter::Type type) :
	m_Position({ x, y }),
	m_Size({ x, y }),
	m_Intensity(intensity),
	m_Type(type)
{
}

void ParticleEmitter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& particle : m_Particles)
	{
		target.draw(*particle);
	}
}

void ParticleEmitter::update()
{
	for (auto& particle : m_Particles)
	{
		particle->update();
	}
	m_Particles.erase(
		std::remove_if(m_Particles.begin(), m_Particles.end(), [](auto& p) { return p->isDead(); }),
		m_Particles.end());
}

sf::Vector2f ParticleEmitter::getPosition() const
{
	return m_Position;
}

void ParticleEmitter::setPosition(sf::Vector2f position)
{
	m_Position = position;
{}}

sf::Vector2f ParticleEmitter::getSize() const
{
	return m_Size;
}

void ParticleEmitter::setSize(sf::Vector2f size)
{
	m_Size = size;
}

float ParticleEmitter::getIntensity() const
{
	return m_Intensity;
}

void ParticleEmitter::setIntensity(float intensity)
{
	m_Intensity = intensity;
}

void ParticleEmitter::generateParticle()
{
	float dx = Random::get(-10, 10);
	float dy = Random::get(-10, 10);
	sf::Time ttl = sf::seconds(Random::get(0, 3));
	//sf::Color color = sf::Color::Red;
	sf::Color color(Random::get(0,255), Random::get(0,255), Random::get(0,255));
	float g = 0;

	if (m_Type == ParticleEmitter::Type::CIRCLE)
	{
		float r = Random::get(6, 10);
		m_Particles.push_back(
			std::make_unique<CircleParticle>(m_Position.x, m_Position.y, r, dx, dy, ttl, color, g));
		return;
	}

	if (m_Type == ParticleEmitter::Type::RECTANGLE)
	{
		float s = Random::get(6, 10);
		m_Particles.push_back(std::make_unique<RectangleParticle>(
			m_Position.x, m_Position.y, s, s, dx, dy, ttl, color, g));
		return;
	}

	if (m_Type == ParticleEmitter::Type::MIXED)
	{
		float s = Random::get(6, 10);
		bool isRectangle = Random::get<bool>();
		if (isRectangle)
		{
			m_Particles.push_back(std::make_unique<RectangleParticle>(m_Position.x, m_Position.y, s, s, dx, dy, ttl, color, g));
		}
		else
		{
			m_Particles.push_back(std::make_unique<CircleParticle>(m_Position.x, m_Position.y, s, dx, dy, ttl, color, g));
		}
		//m_Particles.push_back(particle);
		return;
	}
}




