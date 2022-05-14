#include "ParallaxLayer.h"
#include "Game.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& texture, float distanceFromCamera, float y_offset)
{
	this->texture = texture;
	this->texture.setRepeated(true);
	this->sprite.setTexture(this->texture);
	this->distanceFromCamera = distanceFromCamera;
	this->y_offset = y_offset;

	this->parallaxShader.loadFromMemory(
        "uniform float offsetx;"
		"uniform float offsety;"

        "void main() {"
        "    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;"
        "    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
        "    gl_TexCoord[0].x = gl_TexCoord[0].x + offsetx;" // magic
		"    gl_TexCoord[0].y = gl_TexCoord[0].y + offsety;" // magic
        "    gl_FrontColor = gl_Color;"
        "}"
        , sf::Shader::Vertex);
}

// Inherited via Drawable
void ParallaxLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->sprite, &this->parallaxShader);
}

void ParallaxLayer::setScale(float x, float y)
{
	this->sprite.setScale(x, y);
}

sf::FloatRect ParallaxLayer::getGlobalBounds()
{
	return this->sprite.getGlobalBounds();
}

void ParallaxLayer::update(sf::Vector2f cameraPosition)
{	
	this->sprite.setPosition(cameraPosition.x, cameraPosition.y + this->y_offset);
	parallaxShader.setUniform("offsetx", cameraPosition.x * (this->distanceFromCamera) * 0.0001f);
	//parallaxShader.setUniform("offsety", cameraPosition.y * 1 / (this->distanceFromCamera) * 0.001f);
	parallaxShader.setUniform("offsety", 0.f);
}


