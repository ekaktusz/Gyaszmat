#pragma once

#include "pch.h"
#include "MapLayerNames.h"

class ObjectLayer
{
public:
	ObjectLayer(const tmx::Map& map, MapLayerNames::ObjectLayerName name);
	const std::vector<sf::FloatRect>& getObjectBounds() const;

protected:
	std::vector<sf::FloatRect> m_ObjectBounds;
};