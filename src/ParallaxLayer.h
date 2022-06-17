#pragma once

#include "pch.h"

class ParallaxLayer : public sf::Drawable
{
public:
	ParallaxLayer(const sf::Texture& texture, float distanceFromCamera, float offsetY = 0.f);

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Vector2f cameraPosition);

	void setScale(float x, float y);
	
	sf::FloatRect getGlobalBounds();

private:
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;
	sf::Shader m_ParallaxShader;

	float m_DistanceFromCamera;
	float m_OffsetY;
};