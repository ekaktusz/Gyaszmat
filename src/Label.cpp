#include "Label.h"
#include "ResourceManager.h"

Label::Label()
{}

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->m_Text);
}

void Label::handleEvent(const sf::Event& event)
{}

sf::Text& Label::getText()
{
	return this->m_Text;
}