#pragma once 

#include "cScreen.hpp"

class GameScreen : public cScreen
{
private:

public:
	GameScreen();
	~GameScreen();
	virtual int Run(sf::RenderWindow &App);
};