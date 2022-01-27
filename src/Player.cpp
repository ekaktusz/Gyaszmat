#include "Player.h"

#include <filesystem>

Player::Player()
{
    std::string currentWorkingDirectory = std::filesystem::current_path();
    texture.loadFromFile(currentWorkingDirectory + "/assets/test_player2.png");
    sprite.setTexture(texture);
    sprite.setScale(0.5, 0.5);
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite);
}

void Player::move(sf::Keyboard::Key keyCode)
{
    float GRAVITY = 10;
    float MAX_SPEED = 20;
    float ACCELERATION = 3;
    float motion_x = 0;
    float motion_y = 0;
    float slow = 0.1;
    if (keyCode == sf::Keyboard::W)
    {   
        //TODO: JUMP
    }
    if (keyCode == sf::Keyboard::A)
    {
        if (motion_x + ACCELERATION < MAX_SPEED) motion_x += ACCELERATION;
    }
    else if (keyCode == sf::Keyboard::D)
    {
        if (motion_x - ACCELERATION < -MAX_SPEED) motion_x -= ACCELERATION;
    }
    else
    {
        if (motion_x > 0) {
            motion_x -= slow;
        }
        if (motion_x < 0) {
            motion_x += slow;
        }
    }
    sprite.move(motion_x, motion_y);
}