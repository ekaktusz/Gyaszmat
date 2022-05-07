#pragma once
#include "Widget.h"
#include "Label.h"
#include "ResourceTypes.h"

class Button : public Widget
{
public:
	Button();

	// Text alignment in button
	enum class Alignment
	{
		Center,
		Left,
		Right
	};

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void handleEvent(const sf::Event& event) override;

	void setAction(std::function<void()> actionTodo);
	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
	void setAlignment(Alignment alignment);
	void setText(std::string text);
	void setFont(res::Font font);
	void setInnerColor(sf::Color color);
	void setOutlineColor(sf::Color color);
	void setTextColor(sf::Color color);
	const sf::FloatRect& getGlobalBounds();

private:
	Alignment alignment;
	float outerLineSize = 2;
	float offsetFromEdge = 5;
	sf::RectangleShape innerButton;
	sf::RectangleShape outerButton;
	Label label;
	std::function<void()> actionToDo;
};
