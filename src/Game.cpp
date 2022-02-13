#include "Game.h"

Game::Game() : renderWindow({ Game::XX, Game::YY }, Game::name)
{
	this->map.load(
		(std::filesystem::current_path().parent_path() / "assets" / "platform.tmx").string());
	this->tileLayer = new MapLayer(map, 1);
	this->objectLayer = new MapLayer(map, 3);
	this->renderWindow.setFramerateLimit(Game::FPS);
}

Game::~Game()
{
	delete tileLayer;
	delete objectLayer;
}

void Game::run()
{
	while (this->renderWindow.isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::update()
{
	this->processEvents();
	this->player.update();
	this->updateCollision();
}

void Game::processEvents()
{
	while (this->renderWindow.pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed
			|| sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			this->renderWindow.close();
		}
		this->player.updateKeyboard(this->event);
	}
}

void Game::render()
{
	this->renderWindow.clear();
	this->renderWindow.draw(*this->tileLayer);
	this->renderWindow.draw(this->player);
	this->renderWindow.display();
}

void Game::updateCollision()
{
	sf::FloatRect playerBounds = this->player.getGlobalBounds();
	std::vector<sf::FloatRect> objectBounds = this->objectLayer->getObjectBounds();

	if (playerBounds.top + playerBounds.height > this->renderWindow.getSize().y)
	{
		this->player.stopFalling();
		this->player.setPosition(this->player.getGlobalBounds().left,
			this->renderWindow.getSize().y - this->player.getGlobalBounds().height);
	}

	// collision detection with every object on object layer
	for (const sf::FloatRect& objectBound : objectBounds)
	{
		if (objectBound.intersects(playerBounds))
		{
			// IDK what tod do here, im just messing around
			// here we should implement some hysic
			this->player.stopFalling();
			this->player.setPosition(this->player.getGlobalBounds().left,
				objectBound.top - this->player.getGlobalBounds().height);

			spdlog::debug("Collision detected with objectBounds");
		}
	}
}