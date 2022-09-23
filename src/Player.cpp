#include "Player.h"
#include "ResourceManager.h"
#include "SoundPlayer.h"

// TODO: not the nicest solution
namespace DefaultPlayerAttributes
{
	constexpr short MaxNumberOfJumps = 2;
	constexpr short MaxHealth = 100;
	const sf::Vector2f MaxVelocity = sf::Vector2f(8.f, 32.f); //should be the half of the max tile size
	const sf::Vector2f MinVelocity = sf::Vector2f(1.f, 0.f);
	constexpr float Acceleration = 1.7f;
	constexpr float Drag = 1 - 0.85f;
	constexpr float Gravity = 1.f;
	constexpr float JumpSpeed = -17.f;
	constexpr float MovementModifier = 1;
}


Player::Player(SoundPlayer& soundPlayer) :
	m_Sprite(),
	m_SoundPlayer(soundPlayer),
	m_AnimationComponent(m_Sprite)
{
	m_Health = DefaultPlayerAttributes::MaxHealth;
	m_MaxHealth = DefaultPlayerAttributes::MaxHealth;

	m_AnimationComponent.addAnimation(PlayerAnimationState::Idle, std::make_unique<Animation>(m_Sprite, res::Texture::PlayerIdle, 0.2f));
	m_AnimationComponent.addAnimation(PlayerAnimationState::Jumping, std::make_unique<Animation>(m_Sprite, res::Texture::PlayerJump, 0.3f));
	m_AnimationComponent.addAnimation(PlayerAnimationState::Running, std::make_unique<Animation>(m_Sprite, res::Texture::PlayerRun, 0.1f));
	m_AnimationComponent.addAnimation(PlayerAnimationState::Climbing, std::make_unique<Animation>(m_Sprite, res::Texture::PlayerClimb, 0.3f));

	m_AnimationComponent.setCurrentAnimation(PlayerAnimationState::Idle);

	// Default is to small
	m_Sprite.setScale(2, 2);

	// Set up m_Hitbox
	sf::Vector2f hitboxSize = sf::Vector2f(
		m_Sprite.getGlobalBounds().width - 34,
		m_Sprite.getGlobalBounds().height - 12
	);

	sf::Vector2f hitboxOffSet = sf::Vector2f(17, 12);
	m_Hitbox = Hitbox(m_Sprite.getPosition(), hitboxSize, hitboxOffSet);

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
	//target.draw(light);
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
	updateLight();
}

void Player::handleEvent(sf::Event event)
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
	if (m_Velocity.y != 0)
	{
		m_AnimationState = m_CollisionWithLadder ? PlayerAnimationState::Climbing : PlayerAnimationState::Jumping;
	}
	else
	{
		if (m_Velocity.x != 0)
		{
			m_AnimationState = PlayerAnimationState::Running;
		}
		else
		{
			m_AnimationState = (m_ActualClimbingState == PlayerActualClimbingState::CLIMBED) ? PlayerAnimationState::Climbing : PlayerAnimationState::Idle;
		}
	}

	m_AnimationComponent.setCurrentAnimation(m_AnimationState);

	if (m_AnimationState == PlayerAnimationState::Climbing)
	{
		(m_ActualClimbingState == PlayerActualClimbingState::CLIMBED) ? m_AnimationComponent.pauseAnimation() : m_AnimationComponent.playAnimation();
	}

	// This is for flipping the image to the right direction
	if (m_Velocity.x > 0)
	{
		m_Sprite.setScale(2, 2);
		m_Sprite.setOrigin(0.f, 0.f);
	}
	else if (this->m_Velocity.x < 0)
	{
		m_Sprite.setScale(-2, 2);
		m_Sprite.setOrigin(m_Sprite.getGlobalBounds().width / 2.f, 0);
	}

	m_AnimationComponent.update();
}

void Player::updateSound()
{
	// We should improve it to a better is onGround
	if (m_AnimationState == PlayerAnimationState::Running)
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

void Player::updateLight()
{
	
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
	return m_Velocity.y == 1.f;
}