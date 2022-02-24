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

	int render(sf::RenderWindow& App);
	int update(sf::RenderWindow& App);
	int processEvents(sf::RenderWindow& App);

	virtual int Run(sf::RenderWindow& App);
};