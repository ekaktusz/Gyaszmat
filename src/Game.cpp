#include "Game.h"

const std::string Game::name = "Gyaszmat";

Game::Game() :
	renderWindow({ Game::XX, Game::YY }, Game::name)
{
	std::filesystem::path tmxPath = std::filesystem::current_path().parent_path() / "assets" / "tiles" / "newmaptmx.tmx";
	this->map.load(tmxPath.string());
	this->layerZero = new MapLayer(map, 0);
	this->renderWindow.setFramerateLimit(60);
}

Game::~Game()
{
	delete layerZero;
}

void Game::run()
{
	//MapLayer layerZero(map, 0);
	while (this->renderWindow.isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::update()
{
	while (this->renderWindow.pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			this->renderWindow.close();
		}
		this->player.updateKeyboard(this->event);
	}

	this->player.update();
	this->updateCollision();
}

void Game::render()
{
	this->renderWindow.clear();

	this->renderWindow.draw(*this->layerZero);

	this->renderWindow.draw(this->player);

	this->renderWindow.display();
}

void Game::updateCollision()
{
	if (this->player.getGlobalBounds().top + this->player.getGlobalBounds().height > this->renderWindow.getSize().y)
	{
		this->player.stopFalling();
		this->player.setPosition(
			this->player.getGlobalBounds().left,
			this->renderWindow.getSize().y - this->player.getGlobalBounds().height);
	}
}