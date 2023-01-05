#pragma once

#include "pch.h"
#include "Particle.h"

class ParticleEmitter : public sf::Drawable
{
public:
	// What kind of object does the emitter produces?
	enum class Type
	{
		CIRCLE,
		RECTANGLE,
		MIXED
	};

	ParticleEmitter(
		float x, float y, float sx, float sy, float intensity, ParticleEmitter::Type type);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update();

private:
	Particle* generateParticle();

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Size;
	float m_Intensity;
	std::vector<Particle*> m_Particles;
	ParticleEmitter::Type m_Type;
};