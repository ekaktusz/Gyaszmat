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
			objectBounds.push_back(sf::FloatRect(rect.left, rect.top, rect.width, rect.height));
		}
	}
}

const std::vector<sf::FloatRect>& ObjectLayer::getObjectBounds() const
{
	return this->objectBounds;
}

