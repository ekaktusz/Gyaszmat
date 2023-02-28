#include "TerrainLayer.h"

#include "ObjectLayer.h"
#include "Player.h"

void SolidLayer::updateCollision(Player& player)
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
