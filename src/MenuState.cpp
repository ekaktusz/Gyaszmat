#include "MenuState.h"
#include "ResourceManager.h"
#include "GameState.h"


MenuState::MenuState(Game* game)
{
	this->game = game;
	
	this->titleLabel.getText().setString(game->name);
	this->titleLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Pixel));
	this->titleLabel.getText().setCharacterSize(50);
	this->titleLabel.getText().setPosition(
		this->game->renderWindow.getSize().x - this->titleLabel.getText().getGlobalBounds().width - 20,
		20);


	this->backgroundTexture =
		ResourceManager::getInstance().getTexture(res::Texture::MenuBackground);
	this->background.setTexture(this->backgroundTexture);
	this->startButton.setText("new game");
	this->startButton.setFont(res::Font::Roboto);
	this->startButton.setAlignment(Button::Alignment::Center);
	this->startButton.setPosition(sf::Vector2f(30, 100));
	this->startButton.setSize(sf::Vector2f(200, 30));
	this->startButton.setAction([&]() 
		{ 
			SPDLOG_INFO("BUTTON CLICKED YEAH");
			this->game->pushState(new GameState(this->game));
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
	this->game->renderWindow.draw(startButton);
	this->game->renderWindow.draw(titleLabel);
	this->game->renderWindow.display();
}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		this->game->renderWindow.close();
	}
	this->startButton.handleEvent(event);
}