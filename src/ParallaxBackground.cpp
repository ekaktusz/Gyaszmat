#include "ParallaxBackground.h"
#include "ResourceManager.h"

ParallaxBackground::ParallaxBackground()
{}

ParallaxBackground::~ParallaxBackground()
{
	for (auto layer : backgroundLayers)
	{
		delete layer;
	}
}

void ParallaxBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	for (auto layer : this->backgroundLayers)
	{
		target.draw(*layer);
	}
}

void ParallaxBackground::setScale(float x, float y)
{
	for (auto layer : this->backgroundLayers)
	{
		layer->setScale(x, y);
	}
}

void ParallaxBackground::addLayer(ParallaxLayer* parallaxLayer)
{
	this->backgroundLayers.push_back(parallaxLayer);
}

sf::FloatRect ParallaxBackground::getGlobalBounds()
{
	if (this->backgroundLayers.empty())
	{
		SPDLOG_WARN("backgroundLayers is empty.");
		return sf::FloatRect();
	}
	return backgroundLayers.at(0)->getGlobalBounds();
}


void ParallaxBackground::update(sf::Vector2f cameraPosition)
{
	for (auto layer : this->backgroundLayers)
	{
		layer->update(cameraPosition);
	}
}