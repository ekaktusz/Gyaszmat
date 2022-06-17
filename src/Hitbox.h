#pragma once

#include "pch.h"

class Hitbox : public sf::Drawable
{
public:
	Hitbox();
	Hitbox(const sf::Vector2f& parentPosition, sf::Vector2f size, sf::Vector2f offset);
	~Hitbox();

	void update(const sf::Vector2f& parentPosition);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::FloatRect getGlobalBounds() const;

private:
	sf::RectangleShape m_Hitbox;
	sf::Vector2f m_Offset;
};