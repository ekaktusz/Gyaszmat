#pragma once

#include "Game.h"
#include "pch.h"

class State
{
public:
	Game* game;
	virtual ~State() {};
	virtual void render() = 0;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void handleEvent(const sf::Event& event) = 0;
};