#include "MenuState.h"


MenuState::MenuState()
{

}

void MenuState::update(sf::Time deltaTime)
{

}

void MenuState::render()
{
	this->game->renderWindow.clear();
	this->game->renderWindow.display();
}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->game->renderWindow.close();
	}
}