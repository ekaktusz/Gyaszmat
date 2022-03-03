#include "HealthBar.h"

HealthBar::HealthBar()
{

}

HealthBar::~HealthBar()
{

}


HealthBar::HealthBar(unsigned int health, unsigned int maxHealth) : health(health), maxHealth(maxHealth)
{
	this->offsetFromTopLeft = sf::Vector2f(25.f, 25.f);

	this->healthBar.setSize(sf::Vector2f(240.f, 25.f));
	this->healthBar.setFillColor(sf::Color::Red);
	this->healthBar.setPosition(this->offsetFromTopLeft);

	this->maxHealthBar.setSize(sf::Vector2f(300.f, 25.f));
	this->maxHealthBar.setFillColor(sf::Color(25, 25, 25, 200));
	this->maxHealthBar.setPosition(this->offsetFromTopLeft);
}

void HealthBar::setOffset(const sf::Vector2f& offsetFromTopLeft)
{
	this->offsetFromTopLeft = offsetFromTopLeft;
}

void HealthBar::setPosition(const sf::Vector2f& position)
{
	this->healthBar.setPosition(position + this->offsetFromTopLeft);
	this->maxHealthBar.setPosition(position + this->offsetFromTopLeft);
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(maxHealthBar);
	target.draw(healthBar);
}

void HealthBar::update(unsigned int health)
{
	this->health = health;
}