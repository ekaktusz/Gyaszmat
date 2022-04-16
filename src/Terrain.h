#pragma once

#include "Collidable.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "pch.h"

class Terrain : public Collidable
{
private:
	ObjectLayer* objectLayer;

public:
	Terrain(const tmx::Map* map);
	void updateCollision(Player& player) override;
};
