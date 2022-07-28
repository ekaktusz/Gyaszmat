#pragma once

#include "pch.h"

class CsabRect : public sf::Drawable
{
public:
	CsabRect(float x, float y, float sx, float sy, b2World& world, bool dynamic);

	// Inherited via Drawable
	virtual void draw(RenderTarget& target, RenderStates states) const override;

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