#include "MenuState.h"
#include "GameState.h"
#include "ResourceManager.h"

MenuState::MenuState(Game* game)
{
	this->m_Game = game;

	this->titleLabel.getText().setString(m_Game->s_Name);
	this->titleLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Pixel));
	this->titleLabel.getText().setCharacterSize(50);
	this->titleLabel.getText().setPosition(m_Game->renderWindow.getSize().x
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
		m_Game->pushState(new GameState(m_Game));
	});

	this->exitButton.setText("exitame");
	this->exitButton.setFont(res::Font::Roboto);
	this->exitButton.setAlignment(Button::Alignment::Center);
	this->exitButton.setPosition(sf::Vector2f(30, 200));
	this->exitButton.setSize(sf::Vector2f(200, 30));
	this->exitButton.setAction([&]() {
		SPDLOG_INFO("Exit button clicked");
		m_Game->renderWindow.close();
	});

	this->musicPlayer.chooseTrack(res::Music::LudumDare1);
	this->musicPlayer.setLoop(true);
}

void MenuState::update(sf::Time deltaTime)
{
	this->musicPlayer.play();
	m_Game->renderWindow.setView(m_Game->renderWindow.getDefaultView());
}

void MenuState::render()
{
	m_Game->renderWindow.clear();
	m_Game->renderWindow.draw(background);
	m_Game->renderWindow.draw(startButton);
	m_Game->renderWindow.draw(exitButton);
	m_Game->renderWindow.draw(titleLabel);
	m_Game->renderWindow.display();
}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_Game->renderWindow.close();
	}
	this->startButton.handleEvent(event);
	this->exitButton.handleEvent(event);
}