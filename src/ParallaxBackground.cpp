#include "ParallaxBackground.h"
#include "ResourceManager.h"

ParallaxBackground::ParallaxBackground()
{
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

void ParallaxBackground::setPosition(float x, float y)
{
	for (auto layer : this->backgroundLayers)
	{
		layer->setPosition(x, y);
	}
}

void ParallaxBackground::setPosition(sf::Vector2f position)
{
	for (auto layer : this->backgroundLayers)
	{
		layer->setPosition(position);
	}
}