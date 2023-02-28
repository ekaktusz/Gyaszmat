#pragma once

#include "Collidable.h"
#include "pch.h"
#include "ObjectLayer.h"

class Player;

class SolidLayer : public Collidable, public ObjectLayer
{
public:
	SolidLayer(const tmx::Map& map,
		MapLayerNames::ObjectLayerName name = MapLayerNames::ObjectLayerName::ObjectLayer) :
		ObjectLayer(map, name)
	{
	}
	void updateCollision(Player& player) override;
};
