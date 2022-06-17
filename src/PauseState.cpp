#include "PauseState.h"
#include "ResourceManager.h"

PauseState::PauseState(Game* game)
{
	m_Game = game;

	this->titleLabel.getText().setString(game->s_Name);
	this->titleLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Pixel));
	this->titleLabel.getText().setCharacterSize(50);
	this->titleLabel.getText().setPosition(m_Game->renderWindow.getSize().x / 2.f
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
	this->continueButton.setPosition(sf::Vector2f(m_Game->renderWindow.getSize().x / 2.f
			- this->continueButton.getGlobalBounds().width / 2.f,
		100));
	this->continueButton.setAction([&]() {
		SPDLOG_INFO("Switch back to GameState : Continue the game");
		m_Game->popState();
		throw bool{ false };
	});

	this->mainMenuButton.setText("exit to mainmenu");
	this->mainMenuButton.setFont(res::Font::Roboto);
	this->mainMenuButton.setAlignment(Button::Alignment::Center);
	this->mainMenuButton.setSize(sf::Vector2f(200, 30));
	this->mainMenuButton.setPosition(sf::Vector2f(m_Game->renderWindow.getSize().x / 2.f
			- this->mainMenuButton.getGlobalBounds().width / 2.f,
		200));
	this->mainMenuButton.setAction([&]() { m_Game->returnToMain(); });
}

void PauseState::render()
{
	m_Game->renderWindow.clear();
	m_Game->renderWindow.draw(this->background);
	m_Game->renderWindow.draw(this->continueButton);
	m_Game->renderWindow.draw(this->mainMenuButton);
	m_Game->renderWindow.draw(this->titleLabel);
	m_Game->renderWindow.display();
}

void PauseState::update(sf::Time deltaTime)
{
	m_Game->renderWindow.setView(m_Game->renderWindow.getDefaultView());
}

void PauseState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_Game->renderWindow.close();
	}
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			SPDLOG_INFO("Switch back to GameState : Continue the game");
			m_Game->popState();
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
