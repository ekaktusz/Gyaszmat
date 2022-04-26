#include "GameState.h"
#include "ResourceManager.h"
#include "PauseState.h"
#include "MapLayerNames.h"

GameState::GameState(Game* game)
{
	this->game = game;

	this->map = &ResourceManager::getInstance().getMap(res::Map::TestMap);
	this->tileLayerFar = new TileLayer(*this->map, MapLayerNames::TileLayerName::BackLayer);
	this->tileLayerMiddle = new TileLayer(*this->map, MapLayerNames::TileLayerName::MidLayer);
	this->tileLayerNear = new TileLayer(*this->map, MapLayerNames::TileLayerName::FrontLayer);
	this->ladder = new Ladder(map);
	this->terrain = new Terrain(map);
	this->mapSize = map->getBounds();

	this->view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(Game::XX, Game::YY));
	this->game->renderWindow.setView(this->view);

	this->playerHealthBar.setHealth(100);
	this->playerHealthBar.setMaxHealth(100);

	/*this->parallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxMountain1), 1.f));
	this->parallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxMountain2), 0.97f));
	this->parallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxMountain3), 0.94f));
	this->parallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxMountain4), 0.85f));*/
	this->parallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxMountain5), 0.8f));

	this->parallaxBackground.setScale(Game::YY / this->parallaxBackground.getGlobalBounds().height,
		Game::YY / this->parallaxBackground.getGlobalBounds().height);
}
	GameState::~GameState()
{
	delete tileLayerFar;
	delete tileLayerMiddle;
	delete tileLayerNear;
	delete ladder;
	delete terrain;
	delete map;
}

void GameState::update(sf::Time deltaTime)
{
	this->player.update();
	sf::Vector2f movement =
		player.getCenterPosition() - view.getCenter() - sf::Vector2f(0.f, Game::YY / 10);
	this->view.move(movement * deltaTime.asSeconds() * 10.f);
	//this->view.setCenter(this->player.getCenterPosition() - sf::Vector2f(0.f, Game::XX / 6));
	this->updateCollision();
	this->playerHealthBar.update(this->player.getHealth());

	sf::Vector2f cameraPosition(this->view.getCenter() - sf::Vector2f(Game::XX / 2, Game::YY / 2));
	this->playerHealthBar.setPosition(cameraPosition);

	this->parallaxBackground.update(cameraPosition);
}

void GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		this->game->renderWindow.close();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			SPDLOG_INFO("Switch to PauseState...");
			this->player.stop();
			this->game->pushState(new PauseState(this->game));
		}
	}
	this->player.handleKeyboardInput(event);
}

void GameState::render()
{
	this->game->renderWindow.clear();
	this->game->renderWindow.setView(this->view);

	
	this->game->renderWindow.draw(this->parallaxBackground);

	this->game->renderWindow.draw(*this->tileLayerFar); // layer behind player
	this->game->renderWindow.draw(*this->tileLayerMiddle); // layer of map
	this->game->renderWindow.draw(this->player);
	this->game->renderWindow.draw(*this->tileLayerNear); // layer vefore player
	this->game->renderWindow.draw(this->playerHealthBar);
	this->game->renderWindow.display();
}

void GameState::updateCollision()
{
	this->terrain->updateCollision(this->player);
	this->ladder->updateCollision(this->player);
}