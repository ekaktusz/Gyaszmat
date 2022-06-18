#include "Player.h"
#include "ResourceManager.h"
#include "SoundPlayer.h"

// TODO: not the nicest solution
namespace DefaultPlayerAttributes
{
	const short MaxNumberOfJumps = 2;
	const short MaxHealth = 100;
	const sf::Vector2f MaxVelocity = sf::Vector2f(8.f, 32.f); //should be the half of the max tile size
	const sf::Vector2f MinVelocity = sf::Vector2f(1.f, 0.f);
	const float Acceleration = 1.7f;
	const float Drag = 1 - 0.85f;
	const float Gravity = 1.f;
	const float JumpSpeed = -17.f;
	const float MovementModifier = 1;
}


Player::Player(SoundPlayer& soundPlayer) : m_SoundPlayer(soundPlayer)
{

	m_Health = DefaultPlayerAttributes::MaxHealth;
	m_MaxHealth = DefaultPlayerAttributes::MaxHealth;

	m_IdleTexture = ResourceManager::getInstance().getTexture(res::Texture::PlayerIdle);
	m_RunningTexture = ResourceManager::getInstance().getTexture(res::Texture::PlayerRun);
	m_JumpingTexture = ResourceManager::getInstance().getTexture(res::Texture::PlayerJump);
	m_ClimbingTexture = ResourceManager::getInstance().getTexture(res::Texture::PlayerClimb);

	m_Sprite.setTexture(m_IdleTexture);
	// Set starting frame to the first 32x32 part of the image
	m_CurrentFrame = sf::IntRect(0, 0, 32, 32);
	// Update the sprite texture
    m_Sprite.setTextureRect(m_CurrentFrame);
	// Default is to small
	m_Sprite.setScale(2, 2);

	// Set up m_Hitbox
	sf::Vector2f hitboxSize = sf::Vector2f(
		m_Sprite.getGlobalBounds().width - 34,
		m_Sprite.getGlobalBounds().height - 12
	);
	sf::Vector2f hitboxOffSet = sf::Vector2f(17, 12);
	m_Hitbox = Hitbox(m_Sprite.getPosition(), hitboxSize, hitboxOffSet);

	// Init Animation
	m_AnimationTimer.restart();
	m_AnimationState = PlayerAnimationState::IDLE;
	m_AnimationSwitch = true;

	// Can be played with
	m_Velocity = sf::Vector2f(0.f, 0.f);
	m_MaxVelocity = DefaultPlayerAttributes::MaxVelocity;
	m_MinVelocity = DefaultPlayerAttributes::MinVelocity;
	m_Acceleration = DefaultPlayerAttributes::Acceleration;
	m_Drag = DefaultPlayerAttributes::Drag;
	m_Gravity = DefaultPlayerAttributes::Gravity;
	m_JumpSpeed = DefaultPlayerAttributes::JumpSpeed;
	m_MovementModifier = DefaultPlayerAttributes::MovementModifier;
	m_NumberOfJumps = DefaultPlayerAttributes::MaxNumberOfJumps;

	// For move control
	m_IsMovingLeft = false;
	m_IsMovingRight = false;
	m_PressedJump = false;

	// Health kaland
	m_MaxHealth = 100;
	m_Health = this->m_MaxHealth;

	// Ladder
	m_CollisionWithLadder = false;
	m_Resolved = false;

	m_PossibleClimbingDirection = PlayerPossibleClimbingDir::NONE;
	m_ActualClimbingState = PlayerActualClimbingState::NONE;
}

Player::~Player()
{}

unsigned int Player::getHealth() const
{
	return m_Health;
}

unsigned int Player::getMaxHealth() const
{
	return m_MaxHealth;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite);
	// Uncomment the following row to draw the player's m_Hitbox:
	target.draw(this->m_Hitbox);
}

void Player::update()
{
	updateAnimation();
	updateSound();
	updatePhysics();
	updateHitbox();
}

void Player::handleKeyboardInput(sf::Event event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::W)
		{
			// if we have jump, and not on ladder (or on ladder top)
			if (m_NumberOfJumps > 0 && (!m_CollisionWithLadder || m_PossibleClimbingDirection == PlayerPossibleClimbingDir::DOWN))
			{
				m_SoundPlayer.play(res::Sound::Jump1);
				m_NumberOfJumps--;
				m_PressedJump = true;
			}

			// if on ladder
			if (m_CollisionWithLadder)
			{
				if (m_PossibleClimbingDirection == PlayerPossibleClimbingDir::BOTH || m_PossibleClimbingDirection == PlayerPossibleClimbingDir::UP)
				{
					m_ActualClimbingState = PlayerActualClimbingState::CLIMBINGUP;
					setVelocity(sf::Vector2f(0.f, -3.f));
				}
			}
		}

		if (event.key.code == sf::Keyboard::S)
		{
			if (m_PossibleClimbingDirection == PlayerPossibleClimbingDir::BOTH || m_PossibleClimbingDirection == PlayerPossibleClimbingDir::DOWN)
			{
				this->m_ActualClimbingState = PlayerActualClimbingState::CLIMBINGDOWN;
				this->setVelocity(sf::Vector2f(0.f, 3.f));
			}
		}

		if (event.key.code == sf::Keyboard::A)
		{
			m_IsMovingLeft = true;
			m_IsMovingRight = false;
		}

		else if (event.key.code == sf::Keyboard::D)
		{
			m_IsMovingRight = true;
			m_IsMovingLeft = false;
		}
	}
	else if (event.type == sf::Event::KeyReleased)
	{
		if (event.key.code == sf::Keyboard::A)
		{
			m_IsMovingLeft = false;
		}
		else if (event.key.code == sf::Keyboard::D)
		{
			m_IsMovingRight = false;
		}
		else if ((event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S) && (m_CollisionWithLadder))
		{
			this->m_ActualClimbingState = PlayerActualClimbingState::CLIMBED;
			this->setVelocity(sf::Vector2f(0.f, 0.f));
			stopFalling();
		}
	}
}

void Player::updateAnimation()
{
	PlayerAnimationState prevState = m_AnimationState;

	// Choose the correct animation
	if (m_Velocity.y > 0 && !m_CollisionWithLadder)
		m_AnimationState = PlayerAnimationState::FALLING;
	else if (m_Velocity.y > 0 && m_CollisionWithLadder)
		m_AnimationState = PlayerAnimationState::CLIMBING;
	else if (m_Velocity.y < 0 && !m_CollisionWithLadder)
		m_AnimationState = PlayerAnimationState::JUMPING;
	else if (m_Velocity.y < 0 && m_CollisionWithLadder)
		m_AnimationState = PlayerAnimationState::CLIMBING;
	else
	{
		if (m_Velocity.x > 0)
			m_AnimationState = PlayerAnimationState::MOVING_RIGHT;
		else if (m_Velocity.x < 0)
			m_AnimationState = PlayerAnimationState::MOVING_LEFT;
		else
		{
			if (m_ActualClimbingState == PlayerActualClimbingState::CLIMBED)
			{
				m_AnimationState = PlayerAnimationState::CLIMBING;
			}
			else
			{
				m_AnimationState = PlayerAnimationState::IDLE;
			}
		}
	}

	// If we enter to a new state we have to reset the timer.
	if (prevState != m_AnimationState)
	{
		resetAnimationTimer();
	}

	// Set the correct animation
	if (m_AnimationState == PlayerAnimationState::IDLE)
	{
		setAnimation(0.2f, m_IdleTexture);
	}
	else if (m_AnimationState == PlayerAnimationState::MOVING_RIGHT || m_AnimationState == PlayerAnimationState::MOVING_LEFT)
	{
		setAnimation(0.1f, m_RunningTexture);
	}
	else if (this->m_AnimationState == PlayerAnimationState::JUMPING)
	{
		setAnimation(0.3f, m_JumpingTexture);
	}
	else if (this->m_AnimationState == PlayerAnimationState::FALLING)
	{
		setAnimation(0.3f, m_JumpingTexture);
	}
	else if (this->m_AnimationState == PlayerAnimationState::CLIMBING
		&& this->m_ActualClimbingState == PlayerActualClimbingState::CLIMBED)
	{
		setAnimation(0.3f, m_ClimbingTexture, true);
	}
	else if (this->m_AnimationState == PlayerAnimationState::CLIMBING
		&& this->m_ActualClimbingState != PlayerActualClimbingState::CLIMBED)
	{
		setAnimation(0.3f, m_ClimbingTexture);
	}

	// This is for flipping the image to the right direction
	if (this->m_Velocity.x > 0)
	{
		this->m_Sprite.setScale(2, 2);
		this->m_Sprite.setOrigin(0.f, 0.f);
	}
	else if (this->m_Velocity.x < 0)
	{
		this->m_Sprite.setScale(-2, 2);
		this->m_Sprite.setOrigin(this->m_Sprite.getGlobalBounds().width / 2.f, 0);
	}
}

void Player::updateSound()
{
	// We should improve it to a better is onGround
	if ((m_AnimationState == PlayerAnimationState::MOVING_LEFT || m_AnimationState == PlayerAnimationState::MOVING_RIGHT))
	{
		
		if (this->m_SoundTimer.getElapsedTime().asSeconds() >= 0.3)
		{	
			this->m_SoundPlayer.play(res::Sound::FootStepGrass);
			this->m_SoundTimer.restart();
		}
	}
	else
	{
		this->m_SoundPlayer.removeSoundsById(res::Sound::FootStepGrass);
	}
}

void Player::setAnimation(float timePeriod, sf::Texture& animationTexture, bool stopped)
{
	unsigned int frameSize = 32;
	unsigned int frameNumber = animationTexture.getSize().x / frameSize;
	m_Sprite.setTexture(animationTexture);
	if (m_AnimationTimer.getElapsedTime().asSeconds() >= timePeriod
		|| getAnimationSwitch())
	{
		if (!stopped)
		{
			m_CurrentFrame.left += frameSize;
			if (m_CurrentFrame.left >= (frameNumber - 1) * frameSize)
			{
				m_CurrentFrame.left = 0;
			}
			m_AnimationTimer.restart();
		}
		m_Sprite.setTextureRect(m_CurrentFrame);
	}
}

void Player::updatePhysics()
{
	float deltaTime = 1.f;
	// Movement
	if (m_IsMovingLeft)
		m_Velocity.x += -1.f * m_Acceleration * deltaTime;
	if (m_IsMovingRight)
		m_Velocity.x += 1.f * m_Acceleration * deltaTime;
	if (m_PressedJump)
	{
		m_Velocity.y = m_JumpSpeed;
		m_PressedJump = false;
	}

	if (m_ActualClimbingState != PlayerActualClimbingState::NONE)
	{
		m_Gravity = 0.0f;
		m_MaxVelocity = sf::Vector2f(3.f, 3.f);
	}
	else
	{
		m_Gravity = 1.f;
		m_MaxVelocity = sf::Vector2f(8.f, 32.f);
	}

	// Apply drag and gravitation
	m_Velocity.y += m_Gravity * deltaTime;
	m_Velocity.x *= (1 - m_Drag * deltaTime);

	// Apply bigger drag in air TODO: better, what happens if falling?
	if (m_NumberOfJumps < 2)
		m_Velocity.x *= (1 - m_Drag * deltaTime);

	// Min speed is neccessary cause otherwise it will go slower and slower but never gonna actually stop (drag)
	// Set min speed in the x dimension
	if (std::abs(m_Velocity.x) < m_MinVelocity.x)
		m_Velocity.x = 0.f;

	// Set max speed in the x dimension
	if (std::abs(m_Velocity.x) > m_MaxVelocity.x)
		m_Velocity.x = m_MaxVelocity.x * ((m_Velocity.x < 0.f) ? -1.f : 1.f); // based on directuon

	// Set max speed in the y dimension
	if (std::abs(m_Velocity.y) > m_MaxVelocity.y)
		m_Velocity.y = m_MaxVelocity.y * ((m_Velocity.y < 0.f) ? -1.f : 1.f); // based on directuon


	m_Sprite.move(m_Velocity * deltaTime * m_MovementModifier);
}

void Player::stopFalling()
{
	m_Velocity.y = 0.f;
	m_NumberOfJumps = DefaultPlayerAttributes::MaxNumberOfJumps;
}

void Player::setPosition(const float x, const float y)
{
	m_Sprite.setPosition(x, y);
}

void Player::move(const sf::Vector2f& offset)
{
	m_Sprite.move(offset);
}

// This animation switch mechanism is required for smoother animations: no stutter and flickering
bool Player::getAnimationSwitch()
{
	bool tempAnimationSwitch = m_AnimationSwitch;
	m_AnimationSwitch = false;
	return tempAnimationSwitch;
}

void Player::resetAnimationTimer()
{
	m_AnimationTimer.restart();
	m_AnimationSwitch = true;
}

const Hitbox& Player::getHitbox() const
{
	return m_Hitbox;
}

const sf::Vector2f Player::getCenterPosition() const
{
	float x = m_Hitbox.getGlobalBounds().left + m_Hitbox.getGlobalBounds().width / 2.f;
	float y = m_Hitbox.getGlobalBounds().top + m_Hitbox.getGlobalBounds().height / 2.f;
	return sf::Vector2f(x, y);
}

void Player::setVelocity(sf::Vector2f velocity)
{
	m_Velocity = velocity;
}

const sf::Vector2f Player::getVelocity() const
{
	return m_Velocity;
}

void Player::updateHitbox()
{
	m_Hitbox.update(m_Sprite.getPosition());
}

// This is needed, to stop a player if a game state change happens while pressing down a key
void Player::stop()
{
	m_IsMovingLeft = false;
	m_IsMovingRight = false;
}

const bool Player::isCollidingWithLadder() const
{
	return m_CollisionWithLadder;
}

const bool Player::isResolved() const
{
	return m_Resolved;
}

const PlayerActualClimbingState Player::getActualClimbingState() const
{
	return m_ActualClimbingState;
}

void Player::setCollisionWithLadder(bool collisionWithLadder)
{
	m_CollisionWithLadder = collisionWithLadder;
}

void Player::setResolved(bool newResolvedState)
{
	m_Resolved = newResolvedState;
}

void Player::setPossibleClimbingDirections(PlayerPossibleClimbingDir directionToSet)
{
	m_PossibleClimbingDirection = directionToSet;
}

void Player::setActualClimbingState(PlayerActualClimbingState stateToSet)
{
	m_ActualClimbingState = stateToSet;
}

void Player::resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
	m_Resolved = true;

	//the collision normal is stored in x and y, with the penetration in z
	sf::Vector3f manifold;

	if (overlap.width < overlap.height) // collision in x direction
	{
		manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
		manifold.z = overlap.width;
		m_Velocity = sf::Vector2f(0, m_Velocity.y);
	}
	else // collision in y direction
	{
		manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
		manifold.z = overlap.height;
		stopFalling();
	}

	sf::Vector2f normal(manifold.x * manifold.z, manifold.y * manifold.z);
	move(normal);
}


// TODO: better
bool Player::isOnGround() const
{
	return this->m_Velocity.y == 1.f;
}