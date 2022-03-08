#include "Player.h"

Player::Player()
{
	// Load game assets
	std::filesystem::path idlePlayerAsset = std::filesystem::current_path().parent_path() / "assets"
		/ "owlet_monster" / "Owlet_Monster_Idle_4.png";
	std::filesystem::path runningPlayerAsset = std::filesystem::current_path().parent_path()
		/ "assets" / "owlet_monster" / "Owlet_Monster_Run_6.png";
	std::filesystem::path jumpingPlayerAsset = std::filesystem::current_path().parent_path()
		/ "assets" / "owlet_monster" / "Owlet_Monster_Jump_8.png";
	std::filesystem::path climbingPlayerAsset = std::filesystem::current_path().parent_path()
		/ "assets" / "owlet_monster" / "Owlet_Monster_Climb_4.png";

	this->idleTexture.loadFromFile(idlePlayerAsset.string());
	this->movingTexture.loadFromFile(runningPlayerAsset.string());
	this->jumpingTexture.loadFromFile(jumpingPlayerAsset.string());
	this->climbingTexture.loadFromFile(climbingPlayerAsset.string());

	this->sprite.setTexture(idleTexture);
	// Set starting frame to the first 32x32 part of the image
	this->currentFrame = sf::IntRect(0, 0, 32, 32);
	// Update the sprite texture
	this->sprite.setTextureRect(currentFrame);
	// Default is to small
	this->sprite.setScale(2, 2);

	// Set up hitbox
	sf::Vector2f hitboxSize = sf::Vector2f(
		this->sprite.getGlobalBounds().width - 34, this->sprite.getGlobalBounds().height - 12);
	sf::Vector2f hitboxOffSet = sf::Vector2f(17, 12);
	this->hitbox = Hitbox(this->sprite.getPosition(), hitboxSize, hitboxOffSet);

	// Init Animation
	this->animationTimer.restart();
	this->animationState = PlayerAnimationState::IDLE;
	this->animationSwitch = true;

	// Can be played with
	this->velocity = sf::Vector2f(0.f, 0.f);
	this->maxVelocity = sf::Vector2f(8.f, 32.f); //should be the max of pixel size /2
	this->minVelocity = sf::Vector2f(1.f, 0.f);
	this->acceleration = 1.7f;
	this->drag = 1 - 0.85f;
	this->gravity = 1;
	this->jumpSpeed = -17.f;
	this->movementModifier = 1;
	this->numberOfJumps = Player::MAX_NUMBER_OF_JUMPS;

	// For move control
	this->isMovingLeft = false;
	this->isMovingRight = false;
	this->pressedJump = false;
	this->collisionWithLadder = false;
	this->canClimbDown = false;
	this->canClimbUp = false;
	this->firstClimb = false;
	this->isClimbing = false;
}

Player::~Player()
{}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite);
	// Uncomment the following row to draw the player's hitbox:
	target.draw(this->hitbox);
}

void Player::update()
{
	this->updateAnimation();
	this->updatePhysics();
	this->updateHitbox();
}

void Player::updateKeyboard(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::W && this->numberOfJumps > 0
			&& !this->collisionWithLadder)
		{
			this->numberOfJumps--;
			this->pressedJump = true;
		}
		if (event.key.code == sf::Keyboard::A)
		{
			this->isMovingLeft = true;
			this->isMovingRight = false;
		}
		else if (event.key.code == sf::Keyboard::D)
		{
			this->isMovingRight = true;
			this->isMovingLeft = false;
		}
		if (event.key.code == sf::Keyboard::W && this->collisionWithLadder && this->canClimbUp)
		{
			this->isClimbing = true;
			this->setVelocity(sf::Vector2f(0.f, -3.f));
		}
		else if (event.key.code == sf::Keyboard::S && this->collisionWithLadder
			&& this->canClimbDown)
		{
			this->isClimbing = true;
			this->setVelocity(sf::Vector2f(0.f, 3.f));
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
		else if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
			&& this->collisionWithLadder)
		{
			this->isClimbing = false;
			this->setVelocity(sf::Vector2f(0.f, 0.f));
			stopFalling();
		}
	}
}

void Player::updateAnimation()
{
	PlayerAnimationState prevState = this->animationState;

	if (this->velocity.y > 0 && !this->collisionWithLadder)
		this->animationState = PlayerAnimationState::FALLING;
	else if (this->velocity.y > 0 && this->collisionWithLadder)
		this->animationState = PlayerAnimationState::CLIMBING;
	else if (this->velocity.y < 0 && !this->collisionWithLadder)
		this->animationState = PlayerAnimationState::JUMPING;
	else if (this->velocity.y < 0 && this->collisionWithLadder)
		this->animationState = PlayerAnimationState::CLIMBING;
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
	else if (this->animationState == PlayerAnimationState::MOVING_RIGHT
		|| this->animationState == PlayerAnimationState::MOVING_LEFT)
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
	else if (this->animationState == PlayerAnimationState::CLIMBING)
	{
		setAnimation(0.3f, climbingTexture);
	}
}

void Player::setAnimation(float timePeriod, sf::Texture& animationTexture)
{
	unsigned int frameSize = 32;
	unsigned int frameNumber = animationTexture.getSize().x / frameSize;
	this->sprite.setTexture(animationTexture);
	if (this->animationTimer.getElapsedTime().asSeconds() >= timePeriod
		|| this->getAnimationSwitch())
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
	float deltaTime = 1.f;
	// Movement
	if (this->isMovingLeft)
		this->velocity.x += -1.f * this->acceleration * deltaTime;
	if (this->isMovingRight)
		this->velocity.x += 1.f * this->acceleration * deltaTime;
	if (this->pressedJump)
	{
		this->velocity.y = this->jumpSpeed;
		this->pressedJump = false;
	}

	if (this->collisionWithLadder && !this->isMovingLeft && !this->isMovingRight)
	{
		this->gravity = 0.0f;
		this->maxVelocity = sf::Vector2f(3.f, 3.f);
	}
	else
	{
		this->gravity = 1.f;
		this->maxVelocity = sf::Vector2f(8.f, 32.f);
	}

	// Apply drag and gravitation
	this->velocity.y += this->gravity * deltaTime;
	this->velocity.x *= (1 - drag * deltaTime);

	// Apply bigger drag in air TODO: better, what happens if falling?
	if (numberOfJumps < 2)
		this->velocity.x *= (1 - drag * deltaTime);

	// Min speed is neccessary cause otherwise it will go slower and slower but never gonna actually stop (drag)
	// Set min speed in the x dimension
	if (std::abs(this->velocity.x) < this->minVelocity.x)
		this->velocity.x = 0.f;

	// Set max speed in the x dimension
	if (std::abs(this->velocity.x) > this->maxVelocity.x)
		this->velocity.x =
			this->maxVelocity.x * ((this->velocity.x < 0.f) ? -1.f : 1.f); // based on directuon

	// Set max speed in the y dimension
	if (std::abs(this->velocity.y) > this->maxVelocity.y)
		this->velocity.y =
			this->maxVelocity.y * ((this->velocity.y < 0.f) ? -1.f : 1.f); // based on directuon

	this->sprite.move(this->velocity.x * deltaTime * movementModifier,
		this->velocity.y * deltaTime * movementModifier);
}

void Player::stopFalling()
{
	this->velocity.y = 0.f;
	numberOfJumps = Player::MAX_NUMBER_OF_JUMPS;
}

void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

void Player::move(const sf::Vector2f& offset)
{
	this->sprite.move(offset);
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

const Hitbox& Player::getHitbox() const
{
	return this->hitbox;
}

const sf::Vector2f Player::getCenterPosition() const
{
	float x = this->hitbox.getGlobalBounds().left + this->hitbox.getGlobalBounds().width / 2.f;
	float y = this->hitbox.getGlobalBounds().top + this->hitbox.getGlobalBounds().height / 2.f;
	return sf::Vector2f(x, y);
}

void Player::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}

const sf::Vector2f Player::getVelocity() const
{
	return this->velocity;
}

void Player::updateHitbox()
{
	this->hitbox.update(this->sprite.getPosition());
}