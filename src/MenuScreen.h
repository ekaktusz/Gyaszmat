#pragma once

#include "cScreen.h"

class MenuScreen : public cScreen
{
private:
	bool playing;
	sf::Event event;
	sf::Font font;
	sf::Text menu1;
	sf::Text menu2;
	sf::Text menu3;
	int menu;

public:
	MenuScreen();
	~MenuScreen();
	virtual int Run(sf::RenderWindow &App);
};