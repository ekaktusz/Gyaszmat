#include "Button.h"
#include "ResourceManager.h"

Button::Button() // just for setting default values
{
	// set default color
	m_InnerButton.setFillColor(sf::Color(175, 28, 122));
	m_OuterButton.setFillColor(sf::Color(237, 255, 104));

	// set default font
	m_Label.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Roboto));

	// set default m_Text color
	m_Label.getText().setFillColor(sf::Color::White);

	// set default size
	this->setSize(sf::Vector2f(300, 50));

	m_Alignment = Alignment::Center;

	this->setPosition(sf::Vector2f(50, 50));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_OuterButton);
	target.draw(m_InnerButton);
	target.draw(m_Label);
}

void Button::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (m_OuterButton.getGlobalBounds().contains(
				sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
		{
			if (m_ActionToDo)
			{
				m_ActionToDo();
			}
			else
			{
				SPDLOG_WARN("No action set to button.");
			}
		}
	}
}

void Button::setAction(std::function<void()> actionToDo)
{
	m_ActionToDo = actionToDo;
}

void Button::setPosition(sf::Vector2f position)
{
	m_OuterButton.setPosition(position);
	m_InnerButton.setPosition(position.x + m_OuterLineSize, position.y + m_OuterLineSize);

	float x = 0.f;
	float y = 0.f;

	if (m_Alignment == Alignment::Center)
	{
		x = m_InnerButton.getPosition().x + m_InnerButton.getGlobalBounds().width / 2
			- m_Label.getText().getGlobalBounds().width / 2;
		y = m_InnerButton.getPosition().y + m_InnerButton.getGlobalBounds().height / 2
			- m_Label.getText().getGlobalBounds().height;
	}
	else if (m_Alignment == Alignment::Left)
	{
		x = m_InnerButton.getPosition().x + m_OffsetFromEdge;
		y = m_InnerButton.getPosition().y + m_InnerButton.getGlobalBounds().height / 2
			- m_Label.getText().getGlobalBounds().height;
	}
	else if (m_Alignment == Alignment::Right)
	{
		x = m_InnerButton.getPosition().x + m_InnerButton.getGlobalBounds().width
			- m_Label.getText().getGlobalBounds().width - m_OffsetFromEdge;
		y = m_InnerButton.getPosition().y + m_InnerButton.getGlobalBounds().height / 2
			- m_Label.getText().getGlobalBounds().height;
	}

	m_Label.getText().setPosition(x, y);
}

void Button::setSize(sf::Vector2f size)
{
	m_OuterButton.setSize(size);
	m_InnerButton.setSize(size - sf::Vector2f(m_OuterLineSize * 2, m_OuterLineSize * 2));
	m_Label.getText().setCharacterSize(m_InnerButton.getSize().y - 2);
	setPosition(m_OuterButton.getPosition());
}

void Button::setAlignment(Button::Alignment alignment)
{
	m_Alignment = alignment;
}

void Button::setText(std::string text)
{
	m_Label.getText().setString(text);
	// We should update the position since m_Text size changed
	setPosition(m_OuterButton.getPosition());
}

void Button::setInnerColor(sf::Color color)
{
	m_InnerButton.setFillColor(color);
}

void Button::setOutlineColor(sf::Color color)
{
	m_OuterButton.setFillColor(color);
}

void Button::setFont(res::Font font)
{
	m_Label.getText().setFont(ResourceManager::getInstance().getFont(font));
}

void Button::setTextColor(sf::Color color)
{
	m_Label.getText().setFillColor(color);
}