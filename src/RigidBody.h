#pragma once

#include "pch.h"

class RigidBody : public sf::Drawable
{
public:
	static constexpr float PPM = 32.f; // Defines how many pixels equals 1 meter (Pixel Per Meter)

	RigidBody();
	RigidBody(float x, float y, float sx, float sy, b2World* world, bool dynamic = true); // dynamic defines wether gravity and other forces will apply or not

	sf::Vector2f getPosition(); // returns the sfml coordinates
	const sf::Vector2f& getSize(); // returns the sfml coordinates

	void update(); // Only required for updating the drawable, not must to implement

	b2Body* getBody() // It's only for now
	{
		return m_Body;
	};

private:
	b2Body* m_Body; // Abstract body, physical thing. No need for delete, as the world which is attached to will delete it.
	b2BodyDef m_BodyDef; // A body definition holds all the data needed to construct a rigid body. 
	b2PolygonShape m_Shape; // Shape of the body, will be attached to a body with a fixture. Later as we progress we should consider other shapes then rectangle.
	b2FixtureDef m_Fixture; // Bundles together a body and a bodyshape, represents a true object in the physical world.
	sf::RectangleShape m_RigidBodyRectangleShape; // Only for debugging purposes (draw it)

	sf::Vector2f m_Size; // Maybe get from b2 somehow?
	
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
};