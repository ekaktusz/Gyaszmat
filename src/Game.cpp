#include "Game.h"

Game::Game() : renderWindow({ Game::XX, Game::YY }, Game::name)
{
	this->map.load(
		(std::filesystem::current_path().parent_path() / "assets" / "platform.tmx").string());
	this->tileLayerFar = new MapLayer(map, 0);
	this->tileLayerMiddle = new MapLayer(map, 1);
	this->tileLayerNear = new MapLayer(map, 2);
	this->objectLayer = new MapLayer(map, 3);
	this->view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(Game::XX, Game::YY));
	this->renderWindow.setFramerateLimit(Game::FPS);
}

Game::~Game()
{
	delete tileLayerFar;
	delete tileLayerMiddle;
	delete tileLayerNear;
	delete objectLayer;
}

int Game::run()
{
	int returnvalue = 0;
	while (this->renderWindow.isOpen())
	{
		returnvalue = this->update();
		this->render();
	}
	return returnvalue;
}

int Game::update()
{
	int returnvalue = this->processEvents();
	this->player.update();
	this->updateCollision();
	return returnvalue;
}

int Game::processEvents()
{
	while (this->renderWindow.pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed
			|| (this->event.type == sf::Event::KeyPressed
				&& this->event.key.code == sf::Keyboard::Escape))
		{
			this->renderWindow.close();
			return (-1);
		}
		this->player.updateKeyboard(this->event);
	}
}

void Game::render()
{
	this->renderWindow.clear();
	this->renderWindow.draw(*this->tileLayerFar);	 // layer behind player
	this->renderWindow.draw(*this->tileLayerMiddle); // layer of map
	this->renderWindow.draw(this->player);
	this->renderWindow.draw(*this->tileLayerNear); // layer before player
	this->view.setCenter(this->player.getCenterPosition());
	this->renderWindow.setView(this->view);
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
			auto collisionNormal = sf::Vector2f(objectBound.left, objectBound.top) - sf::Vector2f(playerBound.left, playerBound.top);
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