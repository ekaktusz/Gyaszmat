#include "RigidBody.h"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(float x, float y, float sx, float sy, b2World* world, bool dynamic)
{
	m_RigidBodyRectangleShape = sf::RectangleShape(sf::Vector2f(sx, sy));
	m_RigidBodyRectangleShape.setPosition(x,y);
	m_RigidBodyRectangleShape.setFillColor(sf::Color(0, 255, 0, 128));

	if (dynamic) m_BodyDef.type = b2_dynamicBody;

	m_BodyDef.position.Set((x + sx / 2) / PPM, -(y + sy / 2) / PPM);
	m_Shape.SetAsBox(sx / PPM / 2, sy / PPM / 2); // TODO: later other shapes

	m_Fixture.shape = &m_Shape;
	if (dynamic) m_Fixture.density = 1.0f;
	if (dynamic) m_Fixture.friction = 0.5f;

	m_Body = world->CreateBody(&m_BodyDef);
	m_Body->CreateFixture(&m_Fixture);
	m_Body->SetFixedRotation(true);
}

sf::Vector2f RigidBody::getPosition() const
{
	float x = m_Body->GetPosition().x * PPM;
	float y = -m_Body->GetPosition().y * PPM;
	return sf::Vector2f(x - m_RigidBodyRectangleShape.getSize().x / 2, y - m_RigidBodyRectangleShape.getSize().y / 2);
}

sf::Vector2f RigidBody::getCenterPosition() const
{
	float x = m_Body->GetPosition().x * PPM;
	float y = -m_Body->GetPosition().y * PPM;
	return sf::Vector2f(x, y);
}

const sf::Vector2f& RigidBody::getSize() const
{
	return m_Size;
}

void RigidBody::update()
{
	b2Vec2 vel = m_Body->GetLinearVelocity();
	float newX = vel.x * m_Dampening;
	vel.x = newX;
	m_Body->SetLinearVelocity(vel);
	m_RigidBodyRectangleShape.setPosition(getPosition());
}

void RigidBody::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(m_RigidBodyRectangleShape);
}
