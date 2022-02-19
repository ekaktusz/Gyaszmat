#pragma once

#include "Entity.h"

class Hitbox : public Entity
{
private:
	sf::Vector2f position;
	sf::Vector2f size;
	sf::FloatRect hitbox;
	sf::Vector2f reduceHitboxBy = sf::Vector2f(34, 12);

public:
	Hitbox();
	~Hitbox();

	void setPosition(const sf::Vector2f& topleft);
	void setSize(const sf::Vector2f& size);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	const sf::Vector2f getPosition() const;
	const sf::Vector2f getSize() const;
	const sf::FloatRect getGlobalBounds() const;
};