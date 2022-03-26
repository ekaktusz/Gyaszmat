#include "Game.h"
#include "GameState.h"
#include "MenuState.h"

void Game::pushState(State* state)
{
	this->states.push(state);
}

void Game::popState()
{
	delete this->states.top();
	this->states.pop();
}

void Game::changeState(State* state)
{
	if (!this->states.empty())
		popState();
	pushState(state);
}

State* Game::peekState()
{
	if (this->states.empty())
		return nullptr;

	return this->states.top();
}

void Game::run()
{
	sf::Event event;
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / Game::MAX_FPS);
	while (this->renderWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			while (this->renderWindow.pollEvent(event))
				this->peekState()->handleEvent(event);
			this->peekState()->update(timePerFrame);
		}
		this->peekState()->render();
	}
}

Game::Game() : renderWindow({ Game::XX, Game::YY }, Game::name)
{
	this->pushState(new MenuState(this)); //starting state
}

Game::~Game()
{
	while (!this->states.empty())
		popState();
}
