#include "Player.h"
#include <filesystem>

Player::Player()
{
    std::filesystem::path playerAsset = std::filesystem::current_path() / "assets" / "owlet_monster" / "Owlet_Monster_Idle_4.png";
    std::filesystem::path playerAsset2 = std::filesystem::current_path() / "assets" / "owlet_monster" / "Owlet_Monster_Run_6.png";
    texture.loadFromFile(playerAsset.string());
    movingTexture.loadFromFile(playerAsset2.string());

    sprite.setTexture(texture);
    currentFrame = sf::IntRect(0, 0, 32, 32);
    sprite.setTextureRect(currentFrame);
    sprite.setScale(2, 2);
    animationTimer.restart();
    animationState = PlayerAnimationStates::IDLE;
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}

void Player::update()
{
    animationState = PlayerAnimationStates::IDLE;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        sprite.move(-1.f, 0.f);
        animationState = PlayerAnimationStates::MOVING_LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        sprite.move(1.f, 0.f);
        animationState = PlayerAnimationStates::MOVING_RIGHT;
    }
    
   /* else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        sprite.move(0.f, -1.f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        sprite.move(0.f, 1.f);
    }*/
    
    updateAnimation();
}

void Player::updateAnimation()
{   
    if (animationState == PlayerAnimationStates::IDLE) 
    {
        sprite.setTexture(texture);
        if (animationTimer.getElapsedTime().asSeconds() >= 0.2)
        {
            currentFrame.left += 32;
            if (currentFrame.left >= 96)
            {
                currentFrame.left = 0;
            }
            animationTimer.restart();
            sprite.setTextureRect(currentFrame);
            
        }
    }
    else if (animationState == PlayerAnimationStates::MOVING_RIGHT)
    { 
        sprite.setTexture(movingTexture);
        if (animationTimer.getElapsedTime().asSeconds() >= 0.1)
        {
            currentFrame.left += 32;
            if (currentFrame.left >= 160)
            {
                currentFrame.left = 0;
            }
            animationTimer.restart();
            sprite.setTextureRect(currentFrame);
            sprite.setScale(2, 2);
            sprite.setOrigin(0.f, 0.f);
        }
        
        //sprite.setOrigin(0, 0);
    }
    else if (animationState == PlayerAnimationStates::MOVING_LEFT)
    {
        sprite.setTexture(movingTexture);
        if (animationTimer.getElapsedTime().asSeconds() >= 0.1)
        {
            currentFrame.left += 32;
            if (currentFrame.left >= 160)
            {
                currentFrame.left = 0;
            }
            animationTimer.restart();
            sprite.setTextureRect(currentFrame);
            sprite.setScale(-2, 2);
            sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, 0);
        }
        
        //sprite.setOrigin(sprite.getLocalBounds().width / 2.f, 0);
    }
}