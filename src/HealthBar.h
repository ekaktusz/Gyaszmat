#pragma once

#include "pch.h"

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
	sf::Text text;
	sf::Font& font;

public:
	HealthBar();
	HealthBar(unsigned int health, unsigned int maxHealth);
	~HealthBar();
	void setOffset(const sf::Vector2f& offsetFromTopLeft);
	void setPosition(const sf::Vector2f& position);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(unsigned int health);
};