#pragma once
#include "Widget.h"

class Label : public Widget, public sf::Text
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;
	Label();
	Label(std::string strText, res::Font font, unsigned int textSize, sf::Vector2f position, sf::Color color);

private:
	sf::Text text;
	
};
