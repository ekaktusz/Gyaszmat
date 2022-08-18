#include "TerrainLayer.h"
#include "Player.h"

TerrainLayer::TerrainLayer(const tmx::Map* map, b2World* world) : ObjectLayer(*map, MapLayerNames::ObjectLayerName::ObjectLayer)
{
	for (const sf::FloatRect& objectBound : m_ObjectBounds)
	{
		m_RigidBodies.push_back(RigidBody(objectBound.left, objectBound.top, objectBound.width, objectBound.height, world, false));
	}
}

void TerrainLayer::updateCollision(Player& player)
{
	sf::FloatRect playerBound = player.getHitbox().getGlobalBounds();

	// collision detection with every object on object layer
	sf::FloatRect overlap;
	for (const sf::FloatRect& objectBound : m_ObjectBounds)
	{
		if (objectBound.intersects(playerBound, overlap))
		{
			auto collisionNormal = sf::Vector2f(objectBound.left, objectBound.top)
				- sf::Vector2f(playerBound.left, playerBound.top);
			player.resolveCollision(overlap, collisionNormal);
		}
	}
}

void TerrainLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (const RigidBody& rigidBody : m_RigidBodies)
	{
		target.draw(rigidBody);
	}
}
