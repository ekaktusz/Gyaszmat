#pragma once

#include "Collidable.h"
#include "pch.h"
#include "ObjectLayer.h"

class Player;

class TerrainLayer : public Collidable, public ObjectLayer
{
public:
	TerrainLayer(const tmx::Map& map,
		MapLayerNames::ObjectLayerName name = MapLayerNames::ObjectLayerName::ObjectLayer) :
		ObjectLayer(map, name)
	{
	}
	virtual void updateCollision(Player& player) override;
};
