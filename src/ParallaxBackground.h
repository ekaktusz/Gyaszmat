#pragma once

#include "pch.h"
#include "ParallaxLayer.h"

class ParallaxBackground : public sf::Drawable
{
public:
	ParallaxBackground();
	~ParallaxBackground();

	void addLayer(ParallaxLayer* parallaxLayer);

	sf::FloatRect getGlobalBounds() const;

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void update(sf::Vector2f cameraPosition);

	void setScale(float x, float y);

private:
	std::vector<ParallaxLayer*> m_BackgroundLayers;
};
