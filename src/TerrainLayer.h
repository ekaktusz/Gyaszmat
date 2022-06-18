#pragma once

#include "Collidable.h"
#include "pch.h"

class Player;
class ObjectLayer;

class TerrainLayer : public Collidable
{
public:
	TerrainLayer(const tmx::Map* map);
	virtual void updateCollision(Player& player) override;

private:
	ObjectLayer* m_ObjectLayer;
};
