#include "HealthBar.h"

HealthBar::HealthBar() : font(*(new sf::Font()))
{}

HealthBar::~HealthBar()
{
	delete &font;
}

HealthBar::HealthBar(unsigned int health, unsigned int maxHealth) :
	health(health),
	maxHealth(maxHealth),
	font(*(new sf::Font()))
{
	this->offsetFromTopLeft = sf::Vector2f(25.f, 25.f);

	// Can be played with
	this->length = 250.f;
	this->height = 20.f;

	// inner bar
	this->healthBar.setSize(
		sf::Vector2f(this->health / this->maxHealth * this->length, this->height));
	this->healthBar.setFillColor(sf::Color::Red);
	this->healthBar.setPosition(this->offsetFromTopLeft);

	// outer bar
	this->maxHealthBar.setSize(sf::Vector2f(length, height));
	this->maxHealthBar.setFillColor(sf::Color(25, 25, 25, 200)); // grey
	this->maxHealthBar.setPosition(this->offsetFromTopLeft);

	// text
	std::filesystem::path fontAsset =
		std::filesystem::current_path().parent_path() / "assets" / "fonts" / "Roboto-Regular.ttf";
	if (!this->font.loadFromFile(fontAsset.string()))
	{
		SPDLOG_ERROR("Font loading failed!");
	}
	this->text.setFont((this->font));
	SPDLOG_INFO("FASSZT1");
	// this->text.setString("[" + std::to_string(health) + "/" + std::to_string(maxHealth)
	//	+ "]"); // can be change to std::format after we support c++20
	this->text.setString("FASSZT");
	// this->text.setColor(sf::Color::Blue);
	// this->text.setFillColor(sf::Color::Blue);
	// this->text.setCharacterSize(this->height - 2);
	// this->text.setPosition(this->maxHealthBar.getPosition().x + this->length / 2,
	//	this->maxHealthBar.getPosition().y + 1);
	// this->text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}

void HealthBar::setOffset(const sf::Vector2f& offsetFromTopLeft)
{
	this->offsetFromTopLeft = offsetFromTopLeft;
}

void HealthBar::setPosition(const sf::Vector2f& position)
{
	this->healthBar.setPosition(position + this->offsetFromTopLeft);
	this->maxHealthBar.setPosition(position + this->offsetFromTopLeft);
	//this->text.setPosition(this->maxHealthBar.getPosition().x + this->length / 2,
	//	this->maxHealthBar.getPosition().y + 1);
}

void HealthBar::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	SPDLOG_INFO((this->font.getInfo().family));
	SPDLOG_INFO((this->text.getFont()->getInfo().family));
	target.draw(this->maxHealthBar);
	target.draw(this->healthBar);
	target.draw(this->text);
}

void HealthBar::update(unsigned int health)
{
	this->health = health;
}