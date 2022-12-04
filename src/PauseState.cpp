#include "PauseState.h"
#include "ResourceManager.h"

PauseState::PauseState(Game* game) : State(game)
{
	m_TitleLabel.getText().setString(Game::s_Name);
	m_TitleLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Pixel));
	m_TitleLabel.getText().setCharacterSize(50);
	m_TitleLabel.getText().setPosition(m_Game->renderWindow.getSize().x / 2.f
			- m_TitleLabel.getText().getGlobalBounds().width / 2.f,
		20.f);

	m_BackgroundTexture =
		ResourceManager::getInstance().getTexture(res::Texture::PauseBackground);
	m_BackgroundTexture.setRepeated(true);
	m_Background.setTexture(m_BackgroundTexture);
	m_Background.setScale(1.5, 1.5);

	m_ContinueButton.setText("continue");
	m_ContinueButton.setFont(res::Font::Roboto);
	m_ContinueButton.setAlignment(Button::Alignment::Center);
	m_ContinueButton.setSize(sf::Vector2f(200, 30));
	m_ContinueButton.setPosition(sf::Vector2f(m_Game->renderWindow.getSize().x / 2.f
			- m_ContinueButton.getGlobalBounds().width / 2.f,
		100));
	m_ContinueButton.setAction([&]() {
		SPDLOG_INFO("Switch back to GameState : Continue the game");
		m_Game->popState();
		throw bool{ false };
	});

	m_MainMenuButton.setText("exit to mainmenu");
	m_MainMenuButton.setFont(res::Font::Roboto);
	m_MainMenuButton.setAlignment(Button::Alignment::Center);
	m_MainMenuButton.setSize(sf::Vector2f(200, 30));
	m_MainMenuButton.setPosition(sf::Vector2f(m_Game->renderWindow.getSize().x / 2.f
			- m_MainMenuButton.getGlobalBounds().width / 2.f,
		200));
	m_MainMenuButton.setAction([&]() { m_Game->returnToMain(); });
}

void PauseState::render()
{
	m_Game->renderWindow.clear();
	m_Game->renderWindow.draw(m_Background);
	m_Game->renderWindow.draw(m_ContinueButton);
	m_Game->renderWindow.draw(m_MainMenuButton);
	m_Game->renderWindow.draw(m_TitleLabel);
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
		m_ContinueButton.handleEvent(event);
		m_MainMenuButton.handleEvent(event);
	}
	catch (bool e)
	{
		return;
	}
}
