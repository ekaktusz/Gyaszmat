#include "Game.h"

Game::Game() : renderWindow({ Game::XX, Game::YY }, Game::name)
{
	this->map.load(
		(std::filesystem::current_path().parent_path() / "assets" / "platform.tmx").string());
	this->tileLayerFar = new MapLayer(map, 0);
	this->tileLayerMiddle = new MapLayer(map, 1);
	this->tileLayerNear = new MapLayer(map, 2);
	this->objectLayer = new MapLayer(map, 3);
	this->ladderLayer = new MapLayer(map, 4);
	this->view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(Game::XX, Game::YY));
	this->renderWindow.setFramerateLimit(Game::FPS);
}

Game::~Game()
{
	delete tileLayerFar;
	delete tileLayerMiddle;
	delete tileLayerNear;
	delete objectLayer;
	delete ladderLayer;
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
	this->renderWindow.draw(*this->tileLayerFar);	 // layer behind player
	this->renderWindow.draw(*this->tileLayerMiddle); // layer of map
	this->renderWindow.draw(this->player);
	this->renderWindow.draw(*this->tileLayerNear); // layer vefore player
	this->view.setCenter(this->player.getCenterPosition());
	this->renderWindow.setView(this->view);
	this->renderWindow.display();
}

void Game::updateCollision()
{
	sf::FloatRect playerBound = this->player.getHitbox().getGlobalBounds();
	std::vector<sf::FloatRect> objectBounds = this->objectLayer->getObjectBounds();
	std::vector<sf::FloatRect> ladderBounds = this->ladderLayer->getObjectBounds();

	// collision detection with every object on object layer
	sf::FloatRect overlap;

	for (const sf::FloatRect& ladderBound : ladderBounds)
	{
		if (ladderBound.intersects(playerBound, overlap))
		{
			this->player.collisionWithLadder = true;
			auto collisionNormal = sf::Vector2f(playerBound.left, playerBound.top)
				- sf::Vector2f(ladderBound.left, ladderBound.top);
			SPDLOG_INFO("RRR {} {}", collisionNormal.y, ladderBound.height);
			// 395, -53, 448, 12, 64
			// collisionNormal.y az aljan, a tetejen, a letra magassaga, a hitbox offset, a player magassaga
			if (collisionNormal.y < ladderBound.height - 64 + 12 - 1)
			{
				this->player.canClimbDown = true;
			}
			else
			{
				this->player.canClimbDown = false;
				//this->player.stopFalling();
			}
			if (!this->player.firstClimb)
			{
				//this->player.stopFalling();
				this->player.firstClimb = true;
			}
		}
		else
		{
			this->player.collisionWithLadder = false;
			this->player.firstClimb = false;
			this->player.isClimbing = false;
		}
	}

	for (const sf::FloatRect& objectBound : objectBounds)
	{
		if (objectBound.intersects(playerBound, overlap))
		{
			if (!this->player.isClimbing)
			{
				auto collisionNormal = sf::Vector2f(objectBound.left, objectBound.top)
					- sf::Vector2f(playerBound.left, playerBound.top);
				resolveCollision(overlap, collisionNormal);
			}
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