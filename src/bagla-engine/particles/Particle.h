#pragma once
#include "pch.h"

class Particle : public sf::Drawable
{
public:
	virtual ~Particle() = 0; // This ensures that this class is abstract, can't be initiated
	Particle(float x, float y, float dx, float dy, sf::Time timeToLive, sf::Color color, float gravity);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update();

	bool isDead() const
	{
		return m_Clock.getElapsedTime() > m_TimeToLive;
	}

protected:
	sf::Vector2f m_Position;
	sf::Vector2f m_Delta;
	sf::Time m_TimeToLive;
	sf::Clock m_Clock;
	sf::Time m_CurrentAge;
	sf::Color m_Color;
	float m_Gravity;
	std::unique_ptr<sf::Shape> m_Shape;
};

class CircleParticle : public Particle
{
public:
	CircleParticle(float x, float y, float radius, float dx, float dy, sf::Time timeToLive, sf::Color color, float gravity);

private:
	float m_Radius;
};

class RectangleParticle : public Particle
{
public:
	RectangleParticle(float x, float y, float sx, float sy, float dx, float dy, sf::Time timeToLive, sf::Color color, float gravity);

private: 
	sf::Vector2f m_Size;
};
