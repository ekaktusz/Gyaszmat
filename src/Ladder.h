#pragma once

#include "pch.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "Collidable.h"

// Pontosabb lenne hogy ladderlayer mert ugye a jelenlegi implementáció alapján ez most az összes laddert a mappen magába foglalja
class Ladder : public Collidable
{
private:
	ObjectLayer* midladderLayer;
	ObjectLayer* topLadderLayer;
	ObjectLayer* botLadderLayer;

public:
	Ladder(const tmx::Map* map);
	~Ladder();

	void updateCollision(Player& player);
};