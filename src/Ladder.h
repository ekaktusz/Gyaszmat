#pragma once

#include "pch.h"
#include "Collidable.h"

class ObjectLayer;
class Player;

// Pontosabb lenne hogy ladderlayer mert ugye a jelenlegi implementáció alapján ez most az összes laddert a mappen magába foglalja
class Ladder : public Collidable
{
public:
	Ladder(const tmx::Map* map);
	~Ladder();

	void updateCollision(Player& player);

private:
	ObjectLayer* midladderLayer;
	ObjectLayer* topLadderLayer;
	ObjectLayer* botLadderLayer;
};