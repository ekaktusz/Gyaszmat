#pragma once

#include "Entity.h"

class Player : public Entity
{
private:
    sf::Texture texture;
public:
    sf::Sprite sprite;
    Player();
    ~Player();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
