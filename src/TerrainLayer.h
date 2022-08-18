#pragma once

#include "Collidable.h"
#include "pch.h"
#include "ObjectLayer.h"
#include "RigidBody.h"

class Player;
class ObjectLayer;

class TerrainLayer : public Collidable, public ObjectLayer, public sf::Drawable
{
public:
	TerrainLayer(const tmx::Map* map, b2World* world);
	virtual void updateCollision(Player& player) override;

private:
	std::vector<RigidBody> m_RigidBodies;

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
