#pragma once

#include "pch.h"

class State;

class Game
{
public:
	Game();
	~Game();

	void pushState(State* state);
	void popState();
	void changeState(State* state);
	State* peekState() const;
	void returnToMain();

	void run();

public:
	// Game variables
	static constexpr unsigned int s_WindowSizeX = 800;
	static constexpr unsigned int s_WindowSizeY = 600;
	inline static const std::string s_Name = "Gyaszmat";
	static constexpr unsigned int s_MaxFPS = 60;
	sf::RenderWindow renderWindow;

private:
	std::stack<State*> m_States;
};