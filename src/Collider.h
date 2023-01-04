#pragma once
#include "pch.h"

class Collider
{
public:
	static resolveCollision(Collider& collider1, Collider& collider2);
	const sf::Vector2f& getPosition() const { return m_Position; }
	const sf::Vector2f& getSize() const { return m_Size; }
	bool isStatic() const { return m_Static; }

private:
	static resolveCollisionWithDynamic(Collider& dynamicCollider1, Collider& dynamicCollider2);
	static resolveCollisionWithStatic(Collider& staticCollider, Collider& dynamicCollider);

private:
	bool m_Static = true;
	sf::Vector2f m_Position;
	sf::Vector2f m_Size;
};