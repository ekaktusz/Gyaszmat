#pragma once

#include "pch.h"

class Entity : public sf::Drawable
{
private:
	unsigned int x, y;
	unsigned int sizeX, sizeY; // for now lets assume that everything will be rectangular (?)
public:
	Entity();
	Entity(unsigned int x, unsigned int y);
	~Entity();
};