#include "Label.h"
#include "ResourceManager.h"

Label::Label()
{

}

Label::Label(std::string strText, res::Font font, unsigned int textSize, sf::Vector2f position, sf::Color color)
{
	this->text.setFont(ResourceManager::getInstance().getFont(font));
	this->text.setString(strText);
	this->text.setFillColor(color);
	this->text.setCharacterSize(textSize);
}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->text);
}

void Label::handleEvent(const sf::Event& event)
{
}