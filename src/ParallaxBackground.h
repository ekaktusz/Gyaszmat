#pragma once

#include "pch.h"
#include "ParallaxLayer.h"

class ParallaxBackground : public sf::Drawable
{
private:
	std::vector<ParallaxLayer*> backgroundLayers;

public:
	ParallaxBackground();
	~ParallaxBackground()
	{
		for (auto layer : backgroundLayers)
		{
			delete layer;
		}
	}

	void addLayer(ParallaxLayer* parallaxLayer);

	void setPosition(float x, float y);
	void setPosition(sf::Vector2f position);

	sf::FloatRect getGlobalBounds()
	{
		if (this->backgroundLayers.empty())
		{
			SPDLOG_WARN("backgroundLayers is empty.");
			return sf::FloatRect();
		}
		return backgroundLayers.at(0)->getGlobalBounds();
	}

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update();

	void setScale(float x, float y);


};
