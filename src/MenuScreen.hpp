#pragma once

#include "cScreen.hpp"

class MenuScreen : public cScreen
{
private:
	bool playing;

public:
	MenuScreen();
	~MenuScreen();
	virtual int Run(sf::RenderWindow &App);
};