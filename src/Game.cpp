#include "Game.h"
#include "MenuState.h"

void Game::pushState(State* state)
{
	m_States.push(state);
}

void Game::popState()
{
	delete m_States.top();
	m_States.pop();
}

void Game::changeState(State* state)
{
	if (!m_States.empty())
		popState();
	pushState(state);
}

State* Game::peekState() const
{
	if (m_States.empty())
		return nullptr;

	return m_States.top();
}

void Game::returnToMain()
{
	while (m_States.size() > 1)
		popState();
}

void Game::run()
{
	sf::Event event;
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / Game::s_MaxFPS);
	while (renderWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			while (renderWindow.pollEvent(event))
				peekState()->handleEvent(event);
			peekState()->update(timePerFrame);
		}
		peekState()->render();
	}
}

Game::Game() : renderWindow({ Game::s_WindowSizeX, Game::s_WindowSizeY }, Game::s_Name)
{
	pushState(new MenuState(this)); //starting state
}

Game::~Game()
{
	while (!m_States.empty())
		popState();
}
