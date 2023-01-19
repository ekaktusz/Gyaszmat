#include "Particle.h"

Particle::~Particle()
{}

Particle::Particle(float x, float y, float dx, float dy, sf::Time timeToLive, sf::Color color,
	float gravity) :
	m_Position( { x, y } ),
	m_Delta( { dx,  dy } ),
	m_TimeToLive(timeToLive),
	m_Color(color),
	m_Gravity(gravity)
{
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{ 
	target.draw(*m_Shape);
}

void Particle::update()
{
	float a = m_Clock.getElapsedTime().asMilliseconds() / m_TimeToLive.asSeconds() / 1000;
	m_Color.a = 255 - a * 255;
	m_Shape->setFillColor(m_Color);
	m_Delta.y += m_Gravity;
	m_Position += m_Delta;
	m_Shape->setPosition(m_Position);
}

CircleParticle::CircleParticle(float x, float y, float radius, float dx, float dy,
	sf::Time timeToLive, sf::Color color, float gravity) :
	Particle(x, y, dx, dy, timeToLive, color, gravity),
	m_Radius(radius)
{
	m_Shape = std::make_unique<sf::CircleShape>(m_Radius);
	m_Shape->setFillColor(m_Color);
}

RectangleParticle::RectangleParticle(float x, float y, float sx, float sy, float dx, float dy,
	sf::Time timeToLive, sf::Color color, float gravity) :
	Particle(x, y, dx, dy, timeToLive, color, gravity),
	m_Size({ sx, sy })
{
	m_Shape = std::make_unique<sf::RectangleShape>(m_Size);
	m_Shape->setFillColor(m_Color);
}
