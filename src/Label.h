#pragma once

#include "pch.h"
#include "Widget.h"

class Label : public Widget
{
public:
	Label();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

	// Ugly but less code for now
	// sf::Text is quite similar to what I want to achive here
	sf::Text& getText();

private:
	sf::Text m_Text;
};
