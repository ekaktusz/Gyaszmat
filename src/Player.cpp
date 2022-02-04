#include "Player.h"
#include <filesystem>
#include <iostream>

Player::Player()
{
    // Load game assets
    std::filesystem::path idlePlayerAsset = std::filesystem::current_path() / "assets" / "owlet_monster" / "Owlet_Monster_Idle_4.png";
    std::filesystem::path runningPlayerAsset = std::filesystem::current_path() / "assets" / "owlet_monster" / "Owlet_Monster_Run_6.png";
    std::filesystem::path jumpingPlayerAsset = std::filesystem::current_path() / "assets" / "owlet_monster" / "Owlet_Monster_Jump_8.png";

    this->idleTexture.loadFromFile(idlePlayerAsset.string());
    this->movingTexture.loadFromFile(runningPlayerAsset.string());
    this->jumpingTexture.loadFromFile(jumpingPlayerAsset.string());

    this->sprite.setTexture(idleTexture);
    // Set starting frame to the first 32x32 part of the image
    this->currentFrame = sf::IntRect(0, 0, 32, 32);
    // Update the sprite texture
    this->sprite.setTextureRect(currentFrame);
    // Default is to small
    this->sprite.setScale(2, 2);

    this->animationTimer.restart();
    this->animationState = PlayerAnimationStates::IDLE;
    this->animationSwitch = true;

    // Can be played with
    this->maxVelocity = sf::Vector2f(8.f, 50.f);
    this->minVelocity = sf::Vector2f(1.f, 1.f);
    this->acceleration = 1.7f; 
    this->drag = 0.85f; // this is the default stopping force, the smaller the harder the drag: decceleration
    this->gravity = 1;
    this->jumpSpeed = -20.f;
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}

// This animation switch mechanism is required for smoother animations: no stutter and flickering
bool Player::getAnimationSwitch() 
{
    bool tempAnimationSwitch = this->animationSwitch;
    this->animationSwitch = false;
    return tempAnimationSwitch;
}

void Player::update()
{
    this->updateKeyboard();
    this->updateAnimation();
    this->updatePhysics();
}

void Player::updateAnimation()
{   
    if (this->animationState == PlayerAnimationStates::IDLE)
    {
        this->sprite.setTexture(this->idleTexture);
        if (this->animationTimer.getElapsedTime().asSeconds() >= 0.2 || this->getAnimationSwitch()) // switch frame in every 0.2 seconds
        {
            this->currentFrame.left += 32; // size of a frame in the image
            if (this->currentFrame.left >= 96) // there are 4 frames: 4*32 - 32 = 96
            {
                this->currentFrame.left = 0;
            }
            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
            
        }
    }
    else if (this->animationState == PlayerAnimationStates::MOVING_RIGHT)
    { 
        this->sprite.setTexture(this->movingTexture);
        if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1 || this->getAnimationSwitch()) // switch frame in every 0.1 seconds
        {
            this->currentFrame.left += 32;
            if (this->currentFrame.left >= 160) // there are 6 frames: 6*32 - 32 = 160
            {
                this->currentFrame.left = 0;
            }
            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
            // this is for flipping the image to the right direction
            this->sprite.setScale(2, 2);
            this->sprite.setOrigin(0.f, 0.f);
        }
    }
    else if (this->animationState == PlayerAnimationStates::MOVING_LEFT)
    {
        this->sprite.setTexture(movingTexture);
        if (this->animationTimer.getElapsedTime().asSeconds() >= 0.1 || this->getAnimationSwitch())
        {
            this->currentFrame.left += 32;
            if (this->currentFrame.left >= 160)
            {
                this->currentFrame.left = 0;
            }
            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
            // this is for flipping the image to the right direction
            this->sprite.setScale(-2, 2);
            this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2.f, 0);
        }
    }
    else if (this->animationState == PlayerAnimationStates::JUMPING)
    {
        std::cout << "hello" << std::endl;
        this->sprite.setTexture(jumpingTexture);
        // matek: v_jump + g * t = 0 egyenlet adja meg hogy mikor ér a tetejére. ebbõl t = -v_jump. t itt a frame számot jelenti
        // mivel 60 fps-sel megyünk ez a képlet adja meg hogy milyen tempóba menjen az animáció. valszeg tré és ezen változtatni kéne majd
        // bugzik is rendesen
        if (this->animationTimer.getElapsedTime().asSeconds() >= 2*(-jumpSpeed / 60.f) || this->getAnimationSwitch())
        {
            this->currentFrame.left += 32;
            if (this->currentFrame.left >= 224)
            {
                this->currentFrame.left = 0;
            }
            this->animationTimer.restart();
            this->sprite.setTextureRect(this->currentFrame);
        }
    }
}

void Player::updatePhysics()
{
    this->velocity.y += this->gravity;
    this->velocity.x *= this->drag;

    // This is neccessary cause other wise it will go slower and slower but never gonna actually stop
    if (std::abs(this->velocity.x) < this->minVelocity.x)
        this->velocity.x = 0.f;
    if (std::abs(this->velocity.y) < this->minVelocity.y)
        this->velocity.y = 0.f;

    // Set max speed in the y dimension
    if (std::abs(this->velocity.y) > this->maxVelocity.y)
        this->velocity.y = this->maxVelocity.y * ((this->velocity.y < 0.f) ? -1.f : 1.f); // based on directuon

    // Set max speed in the x dimension
    if (std::abs(this->velocity.x) > this->maxVelocity.x)
        this->velocity.x = this->maxVelocity.x * ((this->velocity.x < 0.f) ? -1.f : 1.f); // based on directuon

    this->sprite.move(this->velocity);
}

const sf::FloatRect Player::getGlobalBounds() const
{
    return this->sprite.getGlobalBounds();
}

void Player::stopFalling()
{
    this->velocity.y = 0.f;
    onGround = true;
}

void Player::setPosition(const float x, const float y)
{
    this->sprite.setPosition(x, y);
}

void Player::move(float x, float y)
{
    this->velocity.x += x * this->acceleration;
    this->velocity.y += y;
}

void Player::resetAnimationTimer() {
    this->animationTimer.restart();
    this->animationSwitch = true;
}

void Player::updateKeyboard()
{
    if (this->onGround) this->animationState = PlayerAnimationStates::IDLE;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && onGround)
    {
        this->onGround = false;
        this->animationState = PlayerAnimationStates::JUMPING;
        this->move(0.f, this->jumpSpeed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        this->move(-1.f, 0.f);
        this->animationState = PlayerAnimationStates::MOVING_LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        this->move(1.f, 0.f);
        this->animationState = PlayerAnimationStates::MOVING_RIGHT;
    }
}
