#include "ParallaxLayer.h"
#include "Game.h"

ParallaxLayer::ParallaxLayer(const sf::Texture& texture, float distanceFromCamera, float offsetY) :
	m_Texture(texture),
	m_DistanceFromCamera(distanceFromCamera),
	m_OffsetY(offsetY)
{
	m_Texture.setRepeated(true);
	m_Sprite.setTexture(m_Texture);

	m_ParallaxShader.loadFromMemory(
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
	target.draw(m_Sprite, &m_ParallaxShader);
}

void ParallaxLayer::setScale(float x, float y)
{
	m_Sprite.setScale(x, y);
}

sf::FloatRect ParallaxLayer::getGlobalBounds()
{
	return m_Sprite.getGlobalBounds();
}

void ParallaxLayer::update(sf::Vector2f cameraPosition)
{	
	m_Sprite.setPosition(cameraPosition.x, cameraPosition.y + m_OffsetY);
	m_ParallaxShader.setUniform("offsetx", cameraPosition.x * m_DistanceFromCamera * 0.0001f);
	//m_ParallaxShader.setUniform("offsety", cameraPosition.y * 1 / m_distanceFromCamera * 0.001f);
	m_ParallaxShader.setUniform("offsety", 0.f);
}


