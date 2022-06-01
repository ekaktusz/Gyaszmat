#pragma once

#include "Entity.h"
#include "Hitbox.h"

class SoundPlayer;

enum class PlayerAnimationState
{
	IDLE = 0,
	MOVING_LEFT,
	MOVING_RIGHT,
	JUMPING,
	FALLING,
	CLIMBING
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

class Player : public Entity
{
private:
	// Game parameters
	unsigned int health;
	unsigned int maxHealth;

	static const short MAX_NUMBER_OF_JUMPS = 2;

	//Sound
	sf::Clock soundTimer;

	// Hitbox
	Hitbox hitbox;

	// Render
	sf::Texture idleTexture;
	sf::Texture runningTexture;
	sf::Texture jumpingTexture;
	sf::Texture climbingTexture;
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
	short numberOfJumps;
	float movementModifier;

	// Controls
	bool isMovingLeft;
	bool isMovingRight;
	bool pressedJump;

	// Ladder
	bool collisionWithLadder;
	bool resolved;
	PlayerPossibleClimbingDir possibleClimbingDirection;
	PlayerActualClimbingState actualClimbingState;

	void resetAnimationTimer();
	void setAnimation(float timePeriod, sf::Texture& animationTexture, bool stopped = false);

	// Update methods
	void updateAnimation();
	void updatePhysics();
	void updateHitbox();
	void updateSound();

	// Not the owner btw. 
	SoundPlayer& soundPlayer;

	bool isOnGround() const;

public:
	Player(SoundPlayer& soundPlayer);
	~Player();

	unsigned int getHealth();
	unsigned int getMaxHealth();

	bool getAnimationSwitch();
	const Hitbox& getHitbox() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void update();
	void handleKeyboardInput(sf::Event event);

	void stopFalling();
	void move(const sf::Vector2f& offset);

	void setPosition(const float x, const float y);
	void setVelocity(sf::Vector2f velocity);

	const sf::Vector2f getVelocity() const;

	const sf::Vector2f getCenterPosition() const;

	void stop();
	// Ladder:
	const bool isCollidingWithLadder() const;
	const bool isResolved() const;
	const PlayerPossibleClimbingDir getPossibleClimbingDirections() const;
	const PlayerActualClimbingState getActualClimbingState() const;

	void setCollisionWithLadder(bool collisionWithLadder);
	void setResolved(bool newResolvedState);
	void setPossibleClimbingDirections(PlayerPossibleClimbingDir directionToSet);
	void setActualClimbingState(PlayerActualClimbingState stateToSet);

	void resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);
};