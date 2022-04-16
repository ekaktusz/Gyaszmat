#include "Label.h"
#include "ResourceManager.h"

Label::Label()
{}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->text);
}

void Label::handleEvent(const sf::Event& event)
{}

sf::Text& Label::getText()
{
	return this->text;
}