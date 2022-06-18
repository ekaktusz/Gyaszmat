#include "TerrainLayer.h"

#include "ObjectLayer.h"
#include "Player.h"

TerrainLayer::TerrainLayer(const tmx::Map* map)
{
	this->m_ObjectLayer = new ObjectLayer(*map, MapLayerNames::ObjectLayerName::ObjectLayer);
}

void TerrainLayer::updateCollision(Player& player)
{
	sf::FloatRect playerBound = player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> objectBounds = this->m_ObjectLayer->getObjectBounds();

	// collision detection with every object on object layer
	sf::FloatRect overlap;
	for (const sf::FloatRect& objectBound : objectBounds)
	{
		if (objectBound.intersects(playerBound, overlap))
		{
			auto collisionNormal = sf::Vector2f(objectBound.left, objectBound.top)
				- sf::Vector2f(playerBound.left, playerBound.top);
			player.resolveCollision(overlap, collisionNormal);
		}
	}
}
