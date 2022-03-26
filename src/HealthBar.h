#pragma once

#include "pch.h"
#include "Button.h"

class HealthBar : public sf::Drawable
{
private:
	unsigned int health;
	unsigned int maxHealth;

	float length;
	float height;

	sf::Vector2f offsetFromTopLeft;

	sf::RectangleShape healthBar;
	sf::RectangleShape maxHealthBar;
	Label label;

	void alignTextToMid();

public:
	HealthBar(unsigned int health = 100, unsigned int maxHealth = 100);
	void setOffset(const sf::Vector2f& offsetFromTopLeft);
	void setPosition(const sf::Vector2f& position);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(unsigned int health);
};