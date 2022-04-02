#pragma once
#include "pch.h"
#include "MapLayerNames.h"

class ObjectLayer
{
private:
	std::vector<sf::FloatRect> objectBounds;

public:
	ObjectLayer(const tmx::Map& map, MapLayerNames::ObjectLayerName name);
	const std::vector<sf::FloatRect>& getObjectBounds() const;
};