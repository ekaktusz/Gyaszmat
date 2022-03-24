#pragma once

#include "pch.h"
#include "Game.h"


class State
{
public:
	Game* game;
	virtual void render() = 0;
	virtual void update(sf::Time deltaTime) = 0;
	virtual void handleEvents() = 0;
};