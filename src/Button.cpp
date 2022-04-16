#include "Button.h"
#include "ResourceManager.h"

Button::Button() // just for setting default values
{
	// set default color
	this->innerButton.setFillColor(sf::Color(175, 28, 122));
	this->outerButton.setFillColor(sf::Color(237, 255, 104));

	// set default font
	this->label.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Roboto));

	// set default text color
	this->label.getText().setFillColor(sf::Color::White);

	// set default size
	this->setSize(sf::Vector2f(300, 50));

	this->alignment = Alignment::Center;

	this->setPosition(sf::Vector2f(50, 50));
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(this->outerButton);
	target.draw(this->innerButton);
	target.draw(this->label);
}

void Button::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (this->outerButton.getGlobalBounds().contains(
				sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
		{
			if (actionToDo)
			{
				actionToDo();
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
	this->actionToDo = actionToDo;
}

void Button::setPosition(sf::Vector2f position)
{
	this->outerButton.setPosition(position);
	this->innerButton.setPosition(position.x + outerLineSize, position.y + outerLineSize);
	if (alignment == Alignment::Center)
	{
		this->label.getText().setPosition(this->innerButton.getPosition().x
				+ this->innerButton.getGlobalBounds().width / 2
				- this->label.getText().getGlobalBounds().width / 2,
			this->innerButton.getPosition().y + this->innerButton.getGlobalBounds().height / 2
				- this->label.getText().getGlobalBounds().height);
	}
	else if (alignment == Alignment::Left)
	{
		this->label.getText().setPosition(this->innerButton.getPosition().x + offsetFromEdge,
			this->innerButton.getPosition().y + this->innerButton.getGlobalBounds().height / 2
				- this->label.getText().getGlobalBounds().height);
	}
	else if (alignment == Alignment::Right)
	{
		this->label.getText().setPosition(this->innerButton.getPosition().x
				+ this->innerButton.getGlobalBounds().width
				- this->label.getText().getGlobalBounds().width - offsetFromEdge,
			this->innerButton.getPosition().y + this->innerButton.getGlobalBounds().height / 2
				- this->label.getText().getGlobalBounds().height);
	}
}

void Button::setSize(sf::Vector2f size)
{
	this->outerButton.setSize(size);
	this->innerButton.setSize(
		size - sf::Vector2f(this->outerLineSize * 2, this->outerLineSize * 2));
	this->label.getText().setCharacterSize(innerButton.getSize().y - 2);
	this->setPosition(outerButton.getPosition());
}

void Button::setAlignment(Button::Alignment alignment)
{
	this->alignment = alignment;
}

void Button::setText(std::string text)
{
	this->label.getText().setString(text);
	// We should update the position since text size changed
	this->setPosition(outerButton.getPosition());
}

void Button::setColor(sf::Color color)
{
	this->innerButton.setFillColor(color);
}

void Button::setOutlineColor(sf::Color color)
{
	this->outerButton.setFillColor(color);
}

void Button::setFont(res::Font font)
{
	this->label.getText().setFont(ResourceManager::getInstance().getFont(font));
}

void Button::setTextColor(sf::Color color)
{
	this->label.getText().setFillColor(color);
}

const sf::FloatRect& Button::getGlobalBounds()
{
	return this->outerButton.getGlobalBounds();
}
