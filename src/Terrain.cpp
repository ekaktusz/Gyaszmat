#include "Terrain.h"

#include "ObjectLayer.h"
#include "Player.h"

Terrain::Terrain(const tmx::Map* map)
{
	this->objectLayer = new ObjectLayer(*map, MapLayerNames::ObjectLayerName::ObjectLayer);
}

void Terrain::updateCollision(Player& player)
{
	sf::FloatRect playerBound = player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> objectBounds = this->objectLayer->getObjectBounds();

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
