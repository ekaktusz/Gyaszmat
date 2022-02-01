#pragma once

#include "Entity.h"

enum PlayerAnimationStates {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING};

class Player : public Entity
{
private:
    sf::Texture texture;
    sf::Texture movingTexture;
    sf::Sprite sprite;
    sf::IntRect currentFrame;
    sf::Clock animationTimer;
    short animationState;
public:
    Player();
    ~Player();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();
    void updateAnimation();
};
