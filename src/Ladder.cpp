#include "Ladder.h"
#include "ObjectLayer.h"
#include "Player.h"

LadderLayer::LadderLayer(const tmx::Map* map) : 
	m_BotLadderLayer(new ObjectLayer(*map, MapLayerNames::ObjectLayerName::BotLadderLayer)),
	m_MidLadderLayer(new ObjectLayer(*map, MapLayerNames::ObjectLayerName::MidLadderLayer)),
	m_TopLadderLayer(new ObjectLayer(*map, MapLayerNames::ObjectLayerName::TopLadderLayer))
{
}

LadderLayer::~LadderLayer()
{
	delete m_MidLadderLayer;
	delete m_TopLadderLayer;
	delete m_BotLadderLayer;
}

void LadderLayer::updateCollision(Player& player)
{
	player.setCollisionWithLadder(false);
	player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::NONE);

	updateCollisionBotLayer(player);
	updateCollisionMidLayer(player);
	updateCollisionTopLayer(player);


	if (!player.isCollidingWithLadder())
	{
		player.setActualClimbingState(PlayerActualClimbingState::NONE);
	}
}

void LadderLayer::updateCollisionBotLayer(Player& player)
{
	sf::FloatRect overlap;
	sf::FloatRect playerBound = player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> botLadderBounds = m_BotLadderLayer->getObjectBounds();

	// Intersection with bottom layer
	for (const sf::FloatRect& ladderBottomBound : botLadderBounds)
	{
		if (ladderBottomBound.intersects(playerBound, overlap))
		{
			player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::UP);
			player.setCollisionWithLadder(true);
		}
	}
}

void LadderLayer::updateCollisionMidLayer(Player& player)
{
	sf::FloatRect overlap;
	sf::FloatRect playerBound = player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> midLadderBounds = m_MidLadderLayer->getObjectBounds();

	for (const sf::FloatRect& ladderBound : midLadderBounds)
	{
		if (ladderBound.intersects(playerBound, overlap))
		{
			player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::BOTH);
			player.setCollisionWithLadder(true);
		}
	}
}

void LadderLayer::updateCollisionTopLayer(Player& player)
{
	sf::FloatRect overlap;
	sf::FloatRect playerBound = player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> topLadderBounds = m_TopLadderLayer->getObjectBounds();

	for (const sf::FloatRect& ladderTopBound : topLadderBounds)
	{
		if (ladderTopBound.intersects(playerBound, overlap))
		{
			player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::BOTH);
			player.setCollisionWithLadder(true);

			if (player.getActualClimbingState() == PlayerActualClimbingState::NONE
				&& !player.isResolved())
			{
				sf::Vector2<float> collisionNormal =
					sf::Vector2f(ladderTopBound.left, ladderTopBound.top)
					- sf::Vector2f(playerBound.left, playerBound.top);
				player.resolveCollision(overlap, collisionNormal);
				player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::DOWN);
				player.setActualClimbingState(PlayerActualClimbingState::NONE);
			}
		}
	}
}
