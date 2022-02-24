
#include "GameScreen.h"
#include "Game.h"

GameScreen::GameScreen()
{}

GameScreen::~GameScreen()
{}

int GameScreen::Run(sf::RenderWindow& App)
{
	Game game;
	game.run();
	
	return 0;
}