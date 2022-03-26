#pragma once
#include "Widget.h"

class Label : public Widget
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;
	Label();

	// Ugly but less code for now
	// sf::Text is quite similar to what I want to achive here
	sf::Text& getText();

private:
	sf::Text text;
};
