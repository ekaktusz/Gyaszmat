#include "ParallaxLayer.h"
#include "Game.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& texture, float distanceFromCamera)
{
	this->texture = texture;
	this->texture.setRepeated(true);
	this->sprite.setTexture(this->texture);
	this->distanceFromCamera = distanceFromCamera;

	this->parallaxShader.loadFromMemory(
        "uniform float offset;"

        "void main() {"
        "    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;"
        "    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
        "    gl_TexCoord[0].x = gl_TexCoord[0].x + offset;" // magic
        "    gl_FrontColor = gl_Color;"
        "}"
        , sf::Shader::Vertex);
}

// Inherited via Drawable
void ParallaxLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprite, &this->parallaxShader);
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
	this->sprite.setPosition(cameraPosition);
	parallaxShader.setUniform("offset", cameraPosition.x * (this->distanceFromCamera) * 0.0001f);
}


