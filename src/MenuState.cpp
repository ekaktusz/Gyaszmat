#include "MenuState.h"
#include "ResourceManager.h"


MenuState::MenuState(Game* game)
{
	this->game = game;
	this->backgroundTexture =
		ResourceManager::getInstance().getTexture(res::Texture::MenuBackground);
	this->background.setTexture(this->backgroundTexture);
	this->button.setText("teszt");
	this->button.setAlignment(Button::Alignment::Center);
	this->button.setPosition(sf::Vector2f(30, 100));
	this->button.setSize(sf::Vector2f(100, 30));
	this->button.setAction([&]() 
		{ 
			SPDLOG_INFO("BUTTON CLICKED YEAH");
		}
	);
}

void MenuState::update(sf::Time deltaTime)
{
	
}

void MenuState::render()
{
	this->game->renderWindow.clear();
	this->game->renderWindow.draw(background);
	this->game->renderWindow.draw(button);
	this->game->renderWindow.display();
}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->game->renderWindow.close();
	}
	this->button.handleEvent(event);
}