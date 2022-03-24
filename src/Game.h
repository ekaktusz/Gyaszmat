#pragma once
#include "pch.h"

class State;

class Game
{
public:

	static const unsigned int XX = 800;
	static const unsigned int YY = 600;
	const std::string name = "Gyaszmat";
	static const unsigned int MAX_FPS = 60;


	void pushState(State* state);
	void popState();
	void changeState(State* state);
	State* peekState();

	void run();

	Game();
	~Game();

	sf::RenderWindow renderWindow;	
private:

	std::stack<State*> states;
};