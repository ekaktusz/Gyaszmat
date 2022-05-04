#pragma once

#include "pch.h"
#include "ParallaxLayer.h"

class ParallaxBackground : public sf::Drawable
{
private:
	std::vector<ParallaxLayer*> backgroundLayers;

public:
	ParallaxBackground();
	~ParallaxBackground();

	void addLayer(ParallaxLayer* parallaxLayer);

	sf::FloatRect getGlobalBounds();

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void update(sf::Vector2f cameraPosition);

	void setScale(float x, float y);
};
