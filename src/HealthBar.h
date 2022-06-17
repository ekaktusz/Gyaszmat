#pragma once

#include "pch.h"
#include "Label.h"

class HealthBar : public sf::Drawable
{
public:
	HealthBar(unsigned int health = 100, unsigned int maxHealth = 100, float length = 250.f, float height = 20.f);
	void setOffset(const sf::Vector2f& offsetFromTopLeft);
	void setPosition(const sf::Vector2f& position);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(unsigned int health);
	void setHealth(unsigned int health);
	void setMaxHealth(unsigned int maxHealth);

private:
	void alignTextToMid();

private:
	unsigned int m_Health;
	unsigned int m_MaxHealth;

	sf::Vector2f m_OffsetFromTopLeft;

	sf::RectangleShape m_HealthBar;
	sf::RectangleShape m_MaxHealthBar;
	Label m_Label;
};