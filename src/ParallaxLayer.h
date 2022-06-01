#pragma once

#include "pch.h"

class ParallaxLayer : public sf::Drawable
{
public:
	ParallaxLayer(const sf::Texture& texture, float distanceFromCamera, float y_offset = 0.f);

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update(sf::Vector2f cameraPosition);

	void setScale(float x, float y);
	
	sf::FloatRect getGlobalBounds();

private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Shader parallaxShader;

	float distanceFromCamera;
	float y_offset;
};