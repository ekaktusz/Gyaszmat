#pragma once

#include "Entity.h"

class Player : public Entity
{
private:
public:
    sf::Texture texture;
    sf::Sprite sprite;
    Player();
    ~Player();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};
