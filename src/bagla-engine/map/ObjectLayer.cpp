#include "ObjectLayer.h"

ObjectLayer::ObjectLayer(const tmx::Map& map, MapLayerNames::ObjectLayerName name)
{
	int idx = static_cast<int>(name);
	const auto& layers = map.getLayers();
	if (map.getOrientation() == tmx::Orientation::Orthogonal && idx < layers.size()
		&& layers[idx]->getType() == tmx::Layer::Type::Object)
	{
		const auto& objectLayer = layers[idx]->getLayerAs<tmx::ObjectGroup>();
		const auto& objects = objectLayer.getObjects();
		for (const auto& object : objects)
		{
			tmx::FloatRect rect = object.getAABB();
			m_ObjectBounds.push_back(sf::FloatRect(rect.left, rect.top, rect.width, rect.height));
		}
	}
	else
	{
		SPDLOG_WARN("Not a valid layer, object layer will be uninitilaized.");
	}
}

const std::vector<sf::FloatRect>& ObjectLayer::getObjectBounds() const
{
	return m_ObjectBounds;
}
