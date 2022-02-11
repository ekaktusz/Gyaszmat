#include "Game.h"

Game::Game() : renderWindow({ 2560, 2000 }, Game::name)
{
	this->renderWindow.setFramerateLimit(Game::FPS);
}

Game::~Game()
{}

void Game::run()
{
	this->map.load(std::filesystem::current_path().parent_path() / "assets" / "platform.tmx");
	MapLayer layer0(map, 0);
	MapLayer layer1(map, 1);
	MapLayer layer2(map, 2);
	MapLayer layer3(map, 3);
	MapLayer layer4(map, 4);

	while (this->renderWindow.isOpen())
	{
		this->update(layer3);
		this->render(layer1);
	}
}

void Game::update(MapLayer& layer)
{
	this->processEvents();
	this->player.update();
	this->updateCollision(layer);
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

void Game::render(MapLayer& layer)
{
	this->renderWindow.clear();
	this->renderWindow.draw(layer);
	this->renderWindow.draw(player);
	this->renderWindow.display();
}

void Game::updateCollision(MapLayer& layer)
{
	sf::FloatRect playerBounds = this->player.getGlobalBounds();
	std::vector<sf::FloatRect> objectBounds = layer.getObjectBounds();

	if (playerBounds.top + playerBounds.height > this->renderWindow.getSize().y)
	{
		this->player.stopFalling();
		this->player.setPosition(this->player.getGlobalBounds().left,
			this->renderWindow.getSize().y - this->player.getGlobalBounds().height);
	}

	// collision detection with every object on object layer
	for (const auto& objectBound : objectBounds)
	{
		if (objectBound.intersects(playerBounds))
		{
			// IDK what tod do here, im just messing around
			// here we should implement some hysic
			this->player.stopFalling();

			std::cout << "RRR ASD" << std::endl;
		}
	}
}