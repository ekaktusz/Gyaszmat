#pragma once

#include "Collidable.h"
#include "pch.h"

class Player;
class ObjectLayer;

class Terrain : public Collidable
{
public:
	Terrain(const tmx::Map* map);
	void updateCollision(Player& player) override;

private:
	ObjectLayer* objectLayer;
};
