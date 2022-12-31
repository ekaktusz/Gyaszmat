#pragma once

#include "pch.h"
#include "Game.h"


class State
{
public:
	State(Game* game) : m_Game(game)
	{}
	virtual ~State() {};

	virtual void render() = 0;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void handleEvent(const sf::Event& event) = 0;

public:
	Game* m_Game;
};


