#pragma  once

#include "pch.h"

#include "Particle.h"

class ParticleEmitter : public sf::Drawable
{
public:
	ParticleEmitter(float x, float y, float sx, float sy, float intensity);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update();

private:
	void addParticle(Particle& particle);

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Size;
	float m_Intensity;


	std::vector<Particle> m_Particles;
};