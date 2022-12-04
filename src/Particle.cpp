#include "Particle.h"

Particle::Particle(float x, float y, float dx, float dy, int maxAge, sf::Color firstColor,
	sf::Color secondColor, float gravity) :
	m_Position({ x, y }),
	m_Delta({ dx, dy }),
	m_MaxAge(maxAge),
	m_FirstColor(firstColor),
	m_SecondColor(secondColor),
	m_Gravity(gravity)
{}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(m_RectangleShape);
}

void Particle::update()
{
	m_Delta.y += m_Gravity;
	m_Position += m_Delta;
	m_RectangleShape.setPosition(m_Position);
	m_RectangleShape.setSize(5, 5);
	m_RectangleShape.setFillColor(sf::Color.Red);
}
