#pragma once

#include "pch.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "Collidable.h"

// Pontosabb lenne hogy ladderlayer mert ugye a jelenlegi implement�ci� alapj�n ez most az �sszes laddert a mappen mag�ba foglalja
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