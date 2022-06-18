#pragma once

#include "Label.h"
#include "ResourceTypes.h"
#include "Widget.h"

class Button : public Widget
{
public:
	Button();

	// Text m_Alignment in button
	enum class Alignment
	{
		Center,
		Left,
		Right
	};

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void handleEvent(const sf::Event& event) override;

	void setAction(std::function<void()> actionTodo);
	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
	void setAlignment(Alignment alignment);
	void setText(std::string text);
	void setFont(res::Font font);
	void setInnerColor(sf::Color color);
	void setOutlineColor(sf::Color color);
	void setTextColor(sf::Color color);

	sf::FloatRect getGlobalBounds() const
	{
		return m_OuterButton.getGlobalBounds();
	}

private:
	Alignment m_Alignment;
	float m_OuterLineSize = 2;
	float m_OffsetFromEdge = 5;
	sf::RectangleShape m_InnerButton;
	sf::RectangleShape m_OuterButton;
	Label m_Label;
	std::function<void()> m_ActionToDo;
};
