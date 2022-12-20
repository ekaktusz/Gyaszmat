#include "ParallaxBackground.h"
#include "ResourceManager.h"

ParallaxBackground::ParallaxBackground()
{}

ParallaxBackground::~ParallaxBackground()
{
	for (auto layer : m_BackgroundLayers)
	{
		delete layer;
	}
}

void ParallaxBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	for (auto layer : m_BackgroundLayers)
	{
		target.draw(*layer);
	}
}

void ParallaxBackground::setScale(float x, float y)
{
	for (auto layer : m_BackgroundLayers)
	{
		layer->setScale(x, y);
	}
}

void ParallaxBackground::addLayer(ParallaxLayer* parallaxLayer)
{
	m_BackgroundLayers.push_back(parallaxLayer);
}

sf::FloatRect ParallaxBackground::getGlobalBounds() const
{
	if (m_BackgroundLayers.empty())
	{
		SPDLOG_WARN("backgroundLayers is empty.");
		return sf::FloatRect();
	}
	return m_BackgroundLayers.at(0)->getGlobalBounds();
}


void ParallaxBackground::update(sf::Vector2f cameraPosition)
{
	for (auto layer : m_BackgroundLayers)
	{
		layer->update(cameraPosition);
	}
}