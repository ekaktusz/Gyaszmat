#pragma once

#include "Hitbox.h"
#include "AnimationComponent.h"

class SoundPlayer;

enum class PlayerAnimationState
{
	Idle = 0,
	Running,
	Jumping,
	Climbing
};

enum class PlayerPossibleClimbingDir
{
	NONE = 0,
	UP,
	DOWN,
	BOTH
};

enum class PlayerActualClimbingState
{
	NONE = 0,
	CLIMBINGUP,
	CLIMBINGDOWN,
	CLIMBED
};

class Player : public sf::Drawable
{
public:
	Player(SoundPlayer& soundPlayer);
	~Player();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void update();
	void handleEvent(sf::Event event);

	unsigned int getHealth() const;
	unsigned int getMaxHealth() const;

	const Hitbox& getHitbox() const;
	const sf::Vector2f getVelocity() const;
	const sf::Vector2f getCenterPosition() const;
	const bool isCollidingWithLadder() const;
	const bool isResolved() const;
	const PlayerActualClimbingState getActualClimbingState() const;

	void setPosition(const float x, const float y);
	void setVelocity(sf::Vector2f velocity);
	void setCollisionWithLadder(bool collisionWithLadder);
	void setResolved(bool newResolvedState);
	void setPossibleClimbingDirections(PlayerPossibleClimbingDir directionToSet);
	void setActualClimbingState(PlayerActualClimbingState stateToSet);

	void resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);
	void stop();
	void stopFalling();
	void move(const sf::Vector2f& offset);

private:
	// Update methods
	void updateAnimation();
	void updatePhysics();
	void updateHitbox();
	void updateSound();
	void updateLight();

	bool isOnGround() const;

private:
	// Game parameters
	unsigned int m_Health;
	unsigned int m_MaxHealth;

	//Sound
	sf::Clock m_SoundTimer; // needed for steps
	SoundPlayer& m_SoundPlayer;

	// Hitbox
	Hitbox m_Hitbox;

	// Render, animation
	sf::Sprite m_Sprite;
	AnimationComponent<PlayerAnimationState> m_AnimationComponent;
	PlayerAnimationState m_AnimationState;

	// Physics
	sf::Vector2f m_Velocity;
	sf::Vector2f m_MaxVelocity;
	sf::Vector2f m_MinVelocity;

	float m_Acceleration;
	float m_Drag;
	float m_Gravity;
	float m_JumpSpeed;
	short m_NumberOfJumps;
	float m_MovementModifier;

	// Controls
	bool m_IsMovingLeft;
	bool m_IsMovingRight;
	bool m_PressedJump;

	// Ladder
	bool m_CollisionWithLadder;
	bool m_Resolved;
	PlayerPossibleClimbingDir m_PossibleClimbingDirection;
	PlayerActualClimbingState m_ActualClimbingState;
};