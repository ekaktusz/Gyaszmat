#pragma once

#include "Entity.h"

class Player : public Entity
{
private:
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Player();
    ~Player();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
