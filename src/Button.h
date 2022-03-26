#pragma once
#include "Widget.h"

class Button : Widget
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

private:
	sf::RectangleShape innerButton;
	sf::RectangleShape outerButton;
};
