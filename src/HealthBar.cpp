#include "HealthBar.h"
#include "ResourceManager.h"

HealthBar::HealthBar(unsigned int health, unsigned int maxHealth, float length, float height) :
	m_Health(health),
	m_MaxHealth(maxHealth)
{
	m_OffsetFromTopLeft = sf::Vector2f(25.f, 25.f);

	// inner bar
	m_HealthBar.setSize(
		sf::Vector2f(m_Health / m_MaxHealth * length, height));
	m_HealthBar.setFillColor(sf::Color::Red);
	m_HealthBar.setPosition(m_OffsetFromTopLeft);

	// outer bar
	m_MaxHealthBar.setSize(sf::Vector2f(length, height));
	m_MaxHealthBar.setFillColor(sf::Color(25, 25, 25, 200));
	m_MaxHealthBar.setPosition(m_OffsetFromTopLeft);

	// m_Text
	m_Label.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Roboto));
	m_Label.getText().setString("[" + std::to_string(health) + "/" + std::to_string(maxHealth) + "]"); // can be change to std::format after we support c++20
	m_Label.getText().setFillColor(sf::Color::White);
	m_Label.getText().setCharacterSize(height - 4);

	alignTextToMid();
}

void HealthBar::setOffset(const sf::Vector2f& offsetFromTopLeft)
{
	m_OffsetFromTopLeft = offsetFromTopLeft;
}

void HealthBar::setPosition(const sf::Vector2f& position)
{
	m_HealthBar.setPosition(position + m_OffsetFromTopLeft);
	m_MaxHealthBar.setPosition(position + m_OffsetFromTopLeft);
	alignTextToMid();
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_MaxHealthBar);
	target.draw(m_HealthBar);
	target.draw(m_Label);
}

void HealthBar::update(unsigned int health)
{
	m_Health = health;
}

void HealthBar::setHealth(unsigned int health)
{
	m_Health = health;
}

void HealthBar::setMaxHealth(unsigned int maxHealth)
{
	m_MaxHealth = maxHealth;
}

void HealthBar::alignTextToMid()
{
	float x = m_MaxHealthBar.getPosition().x + m_MaxHealthBar.getSize().x / 2 - m_Label.getText().getGlobalBounds().width / 2;
	float y = m_MaxHealthBar.getPosition().y;
	m_Label.getText().setPosition(x, y);
}