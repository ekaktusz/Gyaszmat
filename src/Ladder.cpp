#include "Ladder.h"

Ladder::Ladder(const tmx::Map* map)
{
	this->midladderLayer = new ObjectLayer(*map, MapLayerNames::ObjectLayerName::MidLadderLayer);
	this->topLadderLayer = new ObjectLayer(*map, MapLayerNames::ObjectLayerName::TopLadderLayer);
	this->botLadderLayer = new ObjectLayer(*map, MapLayerNames::ObjectLayerName::BotLadderLayer);
}

Ladder::~Ladder()
{
	delete this->midladderLayer;
	delete this->topLadderLayer;
	delete this->botLadderLayer;
}

void Ladder::updateCollision(Player& player)
{
	sf::FloatRect playerBound = player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> midLadderBounds = this->midladderLayer->getObjectBounds();
	std::vector<sf::FloatRect> topLadderBounds = this->topLadderLayer->getObjectBounds();
	std::vector<sf::FloatRect> botLadderBounds = this->botLadderLayer->getObjectBounds();

	sf::FloatRect overlap;

	for (const sf::FloatRect& ladderBottomBound : botLadderBounds)
	{
		if (ladderBottomBound.intersects(playerBound, overlap))
		{
			// a letra aljan allsz, csak felfele mehetsz, nem kell resolve
			player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::UP);
			player.setCollisionWithLadder(true);
		}
	}

	for (const sf::FloatRect& ladderBound : midLadderBounds)
	{
		if (ladderBound.intersects(playerBound, overlap))
		{
			// a letra kozepen allsz, barmerre mehetsz, nem kell resolve
			player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::BOTH);
			player.setCollisionWithLadder(true);
		}
	}

	// collision detection with ladderTop layer
	for (const sf::FloatRect& ladderTopBound : topLadderBounds)
	{
		if (ladderTopBound.intersects(playerBound, overlap))
		{
			// a letra tetejen allsz, barmerre mehetsz, kell resolve ha felette vagy es nem nyomod lefele
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
				player.stopFalling();
			}
		}
	}

	if (!player.isCollidingWithLadder())
	{
		player.setActualClimbingState(PlayerActualClimbingState::NONE);
	}
}