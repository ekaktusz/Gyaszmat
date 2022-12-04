#include "MenuState.h"
#include "GameState.h"
#include "ResourceManager.h"

MenuState::MenuState(Game* game) : State(game)
{
	m_TitleLabel.getText().setString(Game::s_Name);
	m_TitleLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Pixel));
	m_TitleLabel.getText().setCharacterSize(50);
	m_TitleLabel.getText().setPosition(m_Game->renderWindow.getSize().x - m_TitleLabel.getText().getGlobalBounds().width - 20, 20);

	m_BackgroundTexture = ResourceManager::getInstance().getTexture(res::Texture::MenuBackgroundForest);
	m_Background.setTexture(m_BackgroundTexture);

	m_StartButton.setText("new game");
	m_StartButton.setFont(res::Font::Roboto);
	m_StartButton.setAlignment(Button::Alignment::Center);
	m_StartButton.setPosition(sf::Vector2f(30, 100));
	m_StartButton.setSize(sf::Vector2f(200, 30));
	m_StartButton.setAction([&]() {
		SPDLOG_INFO("Switch to GameState: Starting the game");
		m_MusicPlayer.pause();
		m_Game->pushState(new GameState(m_Game));
	});

	m_ExitButton.setText("exitame");
	m_ExitButton.setFont(res::Font::Roboto);
	m_ExitButton.setAlignment(Button::Alignment::Center);
	m_ExitButton.setPosition(sf::Vector2f(30, 200));
	m_ExitButton.setSize(sf::Vector2f(200, 30));
	m_ExitButton.setAction([&]() {
		SPDLOG_INFO("Exit button clicked");
		m_Game->renderWindow.close();
	});

	m_MusicPlayer.chooseTrack(res::Music::LudumDare1);
	m_MusicPlayer.setLoop(true);
}

void MenuState::update(sf::Time deltaTime)
{
	m_MusicPlayer.play();
	m_Game->renderWindow.setView(m_Game->renderWindow.getDefaultView());
}

void MenuState::render()
{
	m_Game->renderWindow.clear();
	m_Game->renderWindow.draw(m_Background);
	m_Game->renderWindow.draw(m_StartButton);
	m_Game->renderWindow.draw(m_ExitButton);
	m_Game->renderWindow.draw(m_TitleLabel);
	m_Game->renderWindow.display();
}

void MenuState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_Game->renderWindow.close();
	}
	m_StartButton.handleEvent(event);
	m_ExitButton.handleEvent(event);
}