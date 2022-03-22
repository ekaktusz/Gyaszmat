#include "Game.h"
#include "ResourceManager.h"

Game::Game() :
	renderWindow({ Game::XX, Game::YY }, Game::name)
{
	this->map = &ResourceManager::getInstance().getMap(res::Map::TestMap);
	this->tileLayerFar = new MapLayer(*map, 0);
	this->tileLayerMiddle = new MapLayer(*map, 1);
	this->tileLayerNear = new MapLayer(*map, 2);
	this->objectLayer = new MapLayer(*map, 3);
	this->view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(Game::XX, Game::YY));
	this->renderWindow.setView(this->view);
	this->playerHealthBar = HealthBar(100, 100);
}

Game::~Game()
{
	delete tileLayerFar;
	delete tileLayerMiddle;
	delete tileLayerNear;
	delete objectLayer;
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / Game::MAX_FPS);
	while (this->renderWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			this->update(timePerFrame);
		}
		this->render();
	}
}

void Game::update(sf::Time deltaTime)
{
	this->player.update();
	sf::Vector2f movement =
		player.getCenterPosition() - view.getCenter() - sf::Vector2f(0.f, Game::XX / 10);
	this->view.move(movement * deltaTime.asSeconds() * 10.f);
	//this->view.setCenter(this->player.getCenterPosition() - sf::Vector2f(0.f, Game::XX / 6));
	this->updateCollision();
	this->playerHealthBar.update(this->player.getHealth());
	this->playerHealthBar.setPosition(
		sf::Vector2f(this->view.getCenter() - sf::Vector2f(Game::XX / 2, Game::YY / 2)));
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
		this->player.handleKeyboardInput(this->event);
	}
}

void Game::render()
{
	this->renderWindow.clear();
	this->renderWindow.setView(this->view);
	this->renderWindow.draw(*this->tileLayerFar);	 // layer behind player
	this->renderWindow.draw(*this->tileLayerMiddle); // layer of map
	this->renderWindow.draw(this->player);
	this->renderWindow.draw(*this->tileLayerNear); // layer vefore player
	this->renderWindow.draw(this->playerHealthBar);
	this->renderWindow.display();
}

void Game::updateCollision()
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

void Game::resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
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