#pragma once

#include "Entity.h"

class Enemy : Entity
{
private:
    /* data */
public:
    Enemy(/* args */);
    ~Enemy();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
