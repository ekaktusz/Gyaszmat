#pragma once

#include "pch.h"

class CsabRect : public sf::Drawable
{
public:
	CsabRect(float x, float y, float sx, float sy, b2World& world, bool dynamic = true);

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void update();

private:
	// b2 staff
	b2Body* body;
	b2BodyDef bodyDef;
	b2PolygonShape bodyShape;
	b2FixtureDef bodyFixture;
	
	float x;
	float y;
	float sx;
	float sy;

	sf::RectangleShape rectangleShape;
};