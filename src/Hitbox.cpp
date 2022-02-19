#include "Hitbox.h"

Hitbox::Hitbox()
{}

Hitbox::~Hitbox()
{}

void Hitbox::setPosition(const sf::Vector2f& topleft)
{
	this->position =
		sf::Vector2f(topleft.x + this->reduceHitboxBy.x / 2.0f, topleft.y + this->reduceHitboxBy.y);
}

void Hitbox::setSize(const sf::Vector2f& size)
{
	this->size = sf::Vector2f(size - this->reduceHitboxBy);
}

void Hitbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	float border = 1;
	sf::RectangleShape hitbox(this->getPosition());
	hitbox.setPosition(this->getPosition());
	hitbox.setSize(this->getSize() - sf::Vector2f(border, border));
	hitbox.setFillColor(sf::Color(0, 0, 0, 0));
	hitbox.setOutlineThickness(border);
	hitbox.setOutlineColor(sf::Color::Red);
	target.draw(hitbox);
}

const sf::Vector2f Hitbox::getPosition() const
{
	return this->position;
}

const sf::Vector2f Hitbox::getSize() const
{
	return this->size;
}

const sf::FloatRect Hitbox::getGlobalBounds() const
{
	sf::FloatRect bounds = sf::FloatRect(
		this->getPosition().x, this->getPosition().y, this->getSize().x, this->getSize().y);
	return bounds;
}
