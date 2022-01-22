#pragma once

#include <SFML/Window.hpp>

class Entity
{
private:
    unsigned int x, y;
    unsigned int sizeX, sizeY; // for now lets assume that everything will be rectangular (?)
public:
    Entity();
    Entity(unsigned int x, unsigned int y);
    ~Entity();
    virtual void draw() = 0;
};