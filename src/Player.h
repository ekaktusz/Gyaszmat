#pragma once

#include "Entity.h"

enum class PlayerAnimationState {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING};

class Player : public Entity
{
private:
    // Render
    sf::Texture idleTexture;
    sf::Texture movingTexture;
    sf::Texture jumpingTexture;
    sf::Sprite sprite;

    // Animation
    sf::IntRect currentFrame;
    sf::Clock animationTimer;
    PlayerAnimationState animationState;
    bool animationSwitch;

    // Physics
    sf::Vector2f velocity;
    sf::Vector2f maxVelocity;
    sf::Vector2f minVelocity;

    float acceleration;
    float drag;
    float gravity;
    float jumpSpeed;
    bool onGround;

    // Methods
    void move(const float x, const float y);
    void updateKeyboard();
    void updateAnimation();
    void updatePhysics();
    void resetAnimationTimer();

public:
    Player();
    ~Player();

    bool getAnimationSwitch();
    const sf::FloatRect getGlobalBounds() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();

    void stopFalling();
    void setPosition(const float x, const float y);
};