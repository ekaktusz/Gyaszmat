#include "GameState.h"
#include "ResourceManager.h"
#include "MenuState.h"

GameState::GameState(Game* game)
{
	this->game = game;
	this->map = &ResourceManager::getInstance().getMap(res::Map::TestMap);
	this->tileLayerFar = new MapLayer(*this->map, 0);
	this->tileLayerMiddle = new MapLayer(*this->map, 1);
	this->tileLayerNear = new MapLayer(*this->map, 2);
	this->objectLayer = new MapLayer(*this->map, 3);
	this->view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(Game::XX, Game::YY));
	this->game->renderWindow.setView(this->view);
	this->playerHealthBar = HealthBar(100, 100);
}

GameState::~GameState()
{
	delete tileLayerFar;
	delete tileLayerMiddle;
	delete tileLayerNear;
	delete objectLayer;
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
	this->playerHealthBar.setPosition(
		sf::Vector2f(this->view.getCenter() - sf::Vector2f(Game::XX / 2, Game::YY / 2)));
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
			SPDLOG_INFO("HELLO");
			this->game->changeState(new MenuState(this->game));
		}
	}
	this->player.handleKeyboardInput(event);
}

void GameState::render()
{
	this->game->renderWindow.clear();
	this->game->renderWindow.setView(this->view);
	this->game->renderWindow.draw(*this->tileLayerFar); // layer behind player
	this->game->renderWindow.draw(*this->tileLayerMiddle); // layer of map
	this->game->renderWindow.draw(this->player);
	this->game->renderWindow.draw(*this->tileLayerNear); // layer vefore player
	this->game->renderWindow.draw(this->playerHealthBar);
	this->game->renderWindow.display();
}

void GameState::updateCollision()
{
	sf::FloatRect playerBound = this->player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> objectBounds = this->objectLayer->getObjectBounds();

	// collision detection with every object on object layer
	sf::FloatRect overlap;
	for (const sf::FloatRect& objectBound : objectBounds)
	{
		if (objectBound.intersects(playerBound, overlap))
		{
			auto collisionNormal = sf::Vector2f(objectBound.left, objectBound.top)
				- sf::Vector2f(playerBound.left, playerBound.top);
			resolveCollision(overlap, collisionNormal);
		}
	}
}

void GameState::resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
	//the collision normal is stored in x and y, with the penetration in z
	sf::Vector3f manifold;

	if (overlap.width < overlap.height) // collision in x direction
	{
		manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
		manifold.z = overlap.width;
		this->player.setVelocity(sf::Vector2f(0, this->player.getVelocity().y));
	}
	else // collision in y direction
	{
		manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
		manifold.z = overlap.height;
		this->player.stopFalling();
	}

	sf::Vector2f normal(manifold.x * manifold.z, manifold.y * manifold.z);
	this->player.move(normal);
}