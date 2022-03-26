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
		SPDLOG_INFO("BUTTON CLICKED YEAH");
		this->game->popState();
	});

}

void PauseState::render()
{
	this->game->renderWindow.clear();
	this->game->renderWindow.draw(this->background);
	this->game->renderWindow.draw(this->continueButton);
	this->game->renderWindow.draw(this->titleLabel);
	this->game->renderWindow.display();
}

void PauseState::update(sf::Time deltaTime)
{
	this->game->renderWindow.setView(this->game->renderWindow.getDefaultView());
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			this->game->popState();
			return;
		}
	}
	this->continueButton.handleEvent(event);
}
