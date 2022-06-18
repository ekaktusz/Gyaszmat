#include "Hitbox.h"

Hitbox::Hitbox()
{}

Hitbox::Hitbox(const sf::Vector2f& parentPosition, sf::Vector2f size, sf::Vector2f offset) :
	m_Hitbox(sf::RectangleShape(size)),
	m_Offset(offset)
{
	m_Hitbox.setPosition(parentPosition + m_Offset);
	m_Hitbox.setFillColor(sf::Color(0, 0, 0, 0));
	m_Hitbox.setOutlineThickness(1.f);
	m_Hitbox.setOutlineColor(sf::Color::Red);
}

Hitbox::~Hitbox()
{}

void Hitbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Hitbox);
}

void Hitbox::update(const sf::Vector2f& parentPosition)
{
	m_Hitbox.setPosition(parentPosition + m_Offset);
}

sf::FloatRect Hitbox::getGlobalBounds() const
{
	return m_Hitbox.getGlobalBounds();
}
