#pragma once

#include "pch.h"
#include "ObjectLayer.h"
#include "Player.h"

// Pontosabb lenne hogy ladderlayer mert ugye a jelenlegi implementáció alapján ez most az összes laddert a mappen magába foglalja
class Ladder
{
private:
	ObjectLayer* midladderLayer;
	ObjectLayer* topLadderLayer;
	ObjectLayer* botLadderLayer;

public:
	Ladder(const tmx::Map* map);
	~Ladder();


	void collide(Player& player);
};