#pragma once

#include "Entity.h"

enum PlayerAnimationStates {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING};

class Player : public Entity
{
private:
    // Render
    sf::Texture idleTexture;
    sf::Texture movingTexture;
    sf::Sprite sprite;

    // Animation
    sf::IntRect currentFrame;
    sf::Clock animationTimer;
    short animationState;
    bool animationSwitch;

    // Physics
    sf::Vector2f velocity;
    sf::Vector2f maxVelocity;
    sf::Vector2f minVelocity;

    float acceleration;
    float drag;
    float gravity;

    // Methods
    void move(const float x, const float y);
    void updateAnimation();
    void updatePhysics();

public:
    Player();
    ~Player();

    bool getAnimationSwitch();
    const sf::FloatRect getGlobalBounds() const;
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();
    
    void stopFalling();
    void setPosition(const float x, const float y);
    void resetAnimationTimer();
};
