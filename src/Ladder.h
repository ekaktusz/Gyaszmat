#pragma once

#include "pch.h"
#include "Collidable.h"

class ObjectLayer;
class Player;

// Pontosabb lenne hogy ladderlayer mert ugye a jelenlegi implement�ci� alapj�n ez most az �sszes laddert a mappen mag�ba foglalja
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