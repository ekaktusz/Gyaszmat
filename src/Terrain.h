#pragma once

#include "Collidable.h"
#include "pch.h"

class Player;
class ObjectLayer;

class Terrain : public Collidable
{
public:
	Terrain(const tmx::Map* map);
	virtual void updateCollision(Player& player) override;

private:
	ObjectLayer* objectLayer;
};
