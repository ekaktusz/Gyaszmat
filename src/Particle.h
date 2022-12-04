#pragma once

#include "pch.h"

// For now it's only rectangles
class Particle : public sf::Drawable
{
public:
	Particle(float x, float y, float dx, float dy, int maxAge, sf::Color firstColor, sf::Color secondColor, float gravity);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update();

private:
	sf::Vector2f m_Position;
	sf::Vector2f m_Size;
	sf::Vector2f m_Delta;

	sf::RectangleShape m_RectangleShape;

	int m_MaxAge;
	int m_Age;

	sf::Color m_FirstColor;
	sf::Color m_SecondColor;

	float m_Gravity;
};