#include "Player.h"

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
    this->animationState = PlayerAnimationState::IDLE;
    this->animationSwitch = true;

    // Can be played with
    this->maxVelocity = sf::Vector2f(8.f, 50.f);
    this->minVelocity = sf::Vector2f(1.f, 1.f);
    this->acceleration = 1.7f; 
    this->drag = 0.85f; // this is the default stopping force, the smaller the harder the drag: decceleration
    this->gravity = 1;
    this->jumpSpeed = -20.f;

    this->isMovingLeft = false;
    this->isMovingRight = false;
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
    this->updateAnimation();
    this->updatePhysics();
}

void Player::updateKeyboard(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::W && onGround)
        {
            this->jump();
        }
        if (event.key.code == sf::Keyboard::A)
        {
            this->isMovingLeft = true;
            this->isMovingRight = false;
            //this->move(-1.f, 0.f);
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            this->isMovingRight = true;
            this->isMovingLeft = false;
            //this->move(1.f, 0.f);
        }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::A)
        {
            this->isMovingLeft = false;
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            this->isMovingRight = false;
        }
    }
}

void Player::updateAnimation()
{   
    PlayerAnimationState prevState = this->animationState;

    if (this->velocity.y > 0)
        this->animationState = PlayerAnimationState::FALLING;
    else if (this->velocity.y < 0)
        this->animationState = PlayerAnimationState::JUMPING;
    else 
    {
        if (this->velocity.x > 0)
            this->animationState = PlayerAnimationState::MOVING_RIGHT;
        else if (this->velocity.x < 0)
            this->animationState = PlayerAnimationState::MOVING_LEFT;
        else
            this->animationState = PlayerAnimationState::IDLE;
    }

    // If we enter to a new state we have to reset the timer.
    if (prevState != this->animationState)
    {
        std::cout << "SANY" << std::endl;
        resetAnimationTimer();
    }

    // This is for flipping the image to the right direction
    if (this->velocity.x > 0) 
    {
        this->sprite.setScale(2, 2);
        this->sprite.setOrigin(0.f, 0.f);
    }
    else if (this->velocity.x < 0)
    {
        this->sprite.setScale(-2, 2);
        this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2.f, 0);
    }


    if (this->animationState == PlayerAnimationState::IDLE)
    {
        setAnimation(0.2f, idleTexture);
    }
    else if (this->animationState == PlayerAnimationState::MOVING_RIGHT ||
        this->animationState == PlayerAnimationState::MOVING_LEFT)
    {
        setAnimation(0.1f, movingTexture);
    }
    else if (this->animationState == PlayerAnimationState::JUMPING)
    {
        setAnimation(0.3f, jumpingTexture);
    }
    else if (this->animationState == PlayerAnimationState::FALLING)
    {
        setAnimation(0.3f, jumpingTexture);
    }
}

void Player::setAnimation(float timePeriod, sf::Texture& animationTexture)
{
    unsigned int frameSize = 32;
    unsigned int frameNumber = animationTexture.getSize().x / frameSize;
    this->sprite.setTexture(animationTexture);
    if (this->animationTimer.getElapsedTime().asSeconds() >= timePeriod || this->getAnimationSwitch())
    {
        this->currentFrame.left += frameSize;
        if (this->currentFrame.left >= (frameNumber - 1) * frameSize)
        {
            this->currentFrame.left = 0;
        }
        this->animationTimer.restart();
        this->sprite.setTextureRect(this->currentFrame);
    }
}

void Player::updatePhysics()
{
    // Movement
    if (this->isMovingLeft)
        this->move(-1.f, 0.f);
    if (this->isMovingRight)
        this->move(1.f, 0.f);

    // Apply drag and gravitation
    this->velocity.y += this->gravity;
    this->velocity.x *= this->drag;
    if (!this->onGround) this->velocity.x *= this->drag; // if in air bigger drag

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

void Player::move(float x, float y)
{
	this->velocity.x += x * this->acceleration;
	this->velocity.y += y;
}

void Player::jump()
{
    this->onGround = false;
    this->move(0.f, this->jumpSpeed);
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

// This animation switch mechanism is required for smoother animations: no stutter and flickering
bool Player::getAnimationSwitch()
{
	bool tempAnimationSwitch = this->animationSwitch;
	this->animationSwitch = false;
	return tempAnimationSwitch;
}

void Player::resetAnimationTimer()
{
	this->animationTimer.restart();
	this->animationSwitch = true;
}

const sf::FloatRect Player::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}
