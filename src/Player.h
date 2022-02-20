#pragma once

#include "Entity.h"
#include "Hitbox.h"

enum class PlayerAnimationState
{
	IDLE = 0,
	MOVING_LEFT,
	MOVING_RIGHT,
	JUMPING,
	FALLING
};

class Player : public Entity
{
private:
	static const short MAX_NUMBER_OF_JUMPS = 2;

	// Hitbox
	Hitbox hitbox;

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
	short numberOfJumps;
	float movementModifier;

	// Controls
	bool isMovingLeft;
	bool isMovingRight;
	bool pressedJump;

	void resetAnimationTimer();
	void setAnimation(float timePeriod, sf::Texture& animationTexture);

	// Update methods
	void updateAnimation();
	void updatePhysics();
	void updateHitbox();

public:
	Player();
	~Player();

	bool getAnimationSwitch();
	const Hitbox& getHitbox() const;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void update();
	void updateKeyboard(sf::Event event);

	void stopFalling();
	void move(const sf::Vector2f& offset);

	void setPosition(const float x, const float y);
	void setVelocity(sf::Vector2f velocity);

	const sf::Vector2f getVelocity() const;

	const sf::Vector2f getCenterPosition() const;
};