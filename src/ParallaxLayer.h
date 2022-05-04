#pragma once

#include "pch.h"

class ParallaxLayer : public sf::Drawable
{
private:
	bool elol = true;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Shader parallaxShader;
	float distanceFromCamera;
	float segg = 0;

public:
	ParallaxLayer(const sf::Texture& texture, float distanceFromCamera);

	// Inherited via Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void setScale(float x, float y);
	
	sf::FloatRect getGlobalBounds();

	void update(sf::Vector2f cameraPosition);

};