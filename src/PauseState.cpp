#include "PauseState.h"
#include "ResourceManager.h"

PauseState::PauseState(Game* game)
{
	this->game = game;

	this->titleLabel.getText().setString(game->name);
	this->titleLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Pixel));
	this->titleLabel.getText().setCharacterSize(50);
	this->titleLabel.getText().setPosition(this->game->renderWindow.getSize().x / 2.f
			- this->titleLabel.getText().getGlobalBounds().width / 2.f,
		20.f);

	this->backgroundTexture =
		ResourceManager::getInstance().getTexture(res::Texture::PauseBackground);
	this->backgroundTexture.setRepeated(true);
	this->background.setTexture(this->backgroundTexture);
	this->background.setScale(1.5, 1.5);

	this->continueButton.setText("continue");
	this->continueButton.setFont(res::Font::Roboto);
	this->continueButton.setAlignment(Button::Alignment::Center);
	this->continueButton.setSize(sf::Vector2f(200, 30));
	this->continueButton.setPosition(sf::Vector2f(this->game->renderWindow.getSize().x / 2.f
			- this->continueButton.getGlobalBounds().width / 2.f,
		100));
	this->continueButton.setAction([&]() {
		SPDLOG_INFO("Switch back to GameState : Continue the game");
		this->game->popState();
		throw bool{ false };
	});

	this->mainMenuButton.setText("exit to mainmenu");
	this->mainMenuButton.setFont(res::Font::Roboto);
	this->mainMenuButton.setAlignment(Button::Alignment::Center);
	this->mainMenuButton.setSize(sf::Vector2f(200, 30));
	this->mainMenuButton.setPosition(sf::Vector2f(this->game->renderWindow.getSize().x / 2.f
			- this->mainMenuButton.getGlobalBounds().width / 2.f,
		200));
	this->mainMenuButton.setAction([&]() { this->game->returnToMain(); });
}

void PauseState::render()
{
	this->game->renderWindow.clear();
	this->game->renderWindow.draw(this->background);
	this->game->renderWindow.draw(this->continueButton);
	this->game->renderWindow.draw(this->mainMenuButton);
	this->game->renderWindow.draw(this->titleLabel);
	this->game->renderWindow.display();
}

void PauseState::update(sf::Time deltaTime)
{
	this->game->renderWindow.setView(this->game->renderWindow.getDefaultView());
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		this->game->renderWindow.close();
	}
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			SPDLOG_INFO("Switch back to GameState : Continue the game");
			this->game->popState();
			return;
		}
	}
	try
	{
		this->continueButton.handleEvent(event);
		this->mainMenuButton.handleEvent(event);
	}
	catch (bool e)
	{
		return;
	}
}
