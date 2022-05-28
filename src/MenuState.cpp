#include "MenuState.h"
#include "GameState.h"
#include "ResourceManager.h"

MenuState::MenuState(Game* game)
{
	this->game = game;

	this->titleLabel.getText().setString(game->name);
	this->titleLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Pixel));
	this->titleLabel.getText().setCharacterSize(50);
	this->titleLabel.getText().setPosition(this->game->renderWindow.getSize().x
			- this->titleLabel.getText().getGlobalBounds().width - 20,
		20);

	this->backgroundTexture =
		ResourceManager::getInstance().getTexture(res::Texture::MenuBackgroundForest);
	this->background.setTexture(this->backgroundTexture);

	this->startButton.setText("new game");
	this->startButton.setFont(res::Font::Roboto);
	this->startButton.setAlignment(Button::Alignment::Center);
	this->startButton.setPosition(sf::Vector2f(30, 100));
	this->startButton.setSize(sf::Vector2f(200, 30));
	this->startButton.setAction([&]() {
		SPDLOG_INFO("Switch to GameState: Starting the game");
		this->musicPlayer.pause();
		this->game->pushState(new GameState(this->game));
	});

	this->exitButton.setText("exitame");
	this->exitButton.setFont(res::Font::Roboto);
	this->exitButton.setAlignment(Button::Alignment::Center);
	this->exitButton.setPosition(sf::Vector2f(30, 200));
	this->exitButton.setSize(sf::Vector2f(200, 30));
	this->exitButton.setAction([&]() {
		SPDLOG_INFO("BUTTON CLICKED YEAH");
		this->game->renderWindow.close();
	});

	this->musicPlayer.chooseTrack(res::Music::LudumDare1);
}

void MenuState::update(sf::Time deltaTime)
{
	this->musicPlayer.play();
	this->game->renderWindow.setView(this->game->renderWindow.getDefaultView());
}

void MenuState::render()
{
	this->game->renderWindow.clear();
	this->game->renderWindow.draw(background);
	this->game->renderWindow.draw(startButton);
	this->game->renderWindow.draw(exitButton);
	this->game->renderWindow.draw(titleLabel);
	this->game->renderWindow.display();
}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		this->game->renderWindow.close();
	}
	this->startButton.handleEvent(event);
	this->exitButton.handleEvent(event);
}