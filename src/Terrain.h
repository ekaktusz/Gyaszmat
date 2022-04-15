#pragma once

#include "pch.h"
#include "Collidable.h"
#include "Objectlayer.h"
#include "Player.h"


class Terrain : public Collidable 
{
private: 
	ObjectLayer* objectLayer;

public: 
	Terrain(const tmx::Map* map);
	void updateCollision(Player& player) override;
};
