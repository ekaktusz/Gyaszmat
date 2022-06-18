#pragma once

#include "pch.h"
#include "ParallaxLayer.h"

class ParallaxBackground : public sf::Drawable
{
public:
	ParallaxBackground();
	~ParallaxBackground();
	
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Vector2f cameraPosition);

	sf::FloatRect getGlobalBounds() const;

	void addLayer(ParallaxLayer* parallaxLayer);
	void setScale(float x, float y);

private:
	std::vector<ParallaxLayer*> m_BackgroundLayers;
};
