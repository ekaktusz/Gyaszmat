#include "Hitbox.h"

Hitbox::Hitbox() 
{}

Hitbox::Hitbox(const sf::Vector2f& parentPosition, sf::Vector2f size, sf::Vector2f offset) :
	offset(offset)
{
	this->hitbox = sf::RectangleShape(size);
	this->hitbox.setPosition(parentPosition + this->offset);
	this->hitbox.setFillColor(sf::Color(0, 0, 0, 0));
	this->hitbox.setOutlineThickness(1.f);
	this->hitbox.setOutlineColor(sf::Color::Red);
}

Hitbox::~Hitbox()
{}

void Hitbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(hitbox);
}

void Hitbox::update(const sf::Vector2f& parentPosition)
{
	this->hitbox.setPosition(parentPosition + this->offset);
}

sf::FloatRect Hitbox::getGlobalBounds() const
{
	return this->hitbox.getGlobalBounds();
}

