#include "ParallaxLayer.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& texture, float distanceFromCamera)
{
	this->texture = texture;
	this->texture.setRepeated(true);
	this->sprite.setTexture(this->texture);
	this->distanceFromCamera = distanceFromCamera;
}

// Inherited via Drawable
void ParallaxLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprite);
}

void ParallaxLayer::setPosition(float x, float y)
{
	this->sprite.setPosition(x, y);
}

void ParallaxLayer::setPosition(sf::Vector2f position)
{
	this->sprite.setPosition(position);
}

void ParallaxLayer::setScale(float x, float y)
{
	this->sprite.setScale(x, y);
}

sf::FloatRect ParallaxLayer::getGlobalBounds()
{
	return this->sprite.getGlobalBounds();
}

const sf::Vector2f& ParallaxLayer::getPosition()
{
	return this->sprite.getPosition();
}

void ParallaxLayer::update(sf::Vector2f cameraPosition)
{
	this->sprite.setPosition(cameraPosition.x * (this->distanceFromCamera), cameraPosition.y);
}


