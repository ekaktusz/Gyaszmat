#include "CsabRect.h"

namespace
{
	float PPM = 32.f;
}

CsabRect::CsabRect(float x, float y, float sx, float sy, b2World& world, bool dynamic)
{
	rectangleShape = sf::RectangleShape(sf::Vector2f(sx, sy));
	rectangleShape.setOrigin(sx / 2, sy / 2);
	rectangleShape.setPosition(x, y);
	rectangleShape.setFillColor(sf::Color(0, 255, 0, 128));

	if (dynamic) bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x/PPM, y/PPM);

	bodyShape.SetAsBox(sx/PPM, sy/PPM);

	bodyFixture.shape = &bodyShape;
	if (dynamic) bodyFixture.density = 1.0f;
	if (dynamic) bodyFixture.friction = 0.5f;

	body = world.CreateBody(&bodyDef);
	body->CreateFixture(&bodyFixture);
}

void CsabRect::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rectangleShape);
}

void CsabRect::update()
{
	float x = body->GetPosition().x * PPM;
	float y = - (body->GetPosition().y * PPM);
	rectangleShape.setPosition(x, y);
}