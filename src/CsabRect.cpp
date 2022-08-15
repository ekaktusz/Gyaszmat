#include "CsabRect.h"

namespace
{
	float PPM = 32.f; // = 1 meter
}

CsabRect::CsabRect(float x, float y, float sx, float sy, b2World& world, bool dynamic)
{
	rectangleShape = sf::RectangleShape(sf::Vector2f(sx, sy));
	rectangleShape.setPosition(x, y);
	rectangleShape.setOrigin(sx / 2, sy / 2);
	rectangleShape.setFillColor(sf::Color(0, 255, 0, 128));
	
	if (dynamic) bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(x/PPM, -y/PPM);
	
	bodyShape.SetAsBox(sx/PPM/2, sy/PPM/2);
	
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
	float y = -body->GetPosition().y * PPM;
	//x = x - rectangleShape.getSize().x / 2.f;
	//y = y - rectangleShape.getSize().y / 2.f;
	rectangleShape.setPosition(x, y);
}