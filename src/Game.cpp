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
	this->ladderTopLayer = new MapLayer(map, 5);
	this->ladderBottomLayer = new MapLayer(map, 6);
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
	delete ladderTopLayer;
	delete ladderBottomLayer;
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
	std::vector<sf::FloatRect> ladderTopBounds = this->ladderTopLayer->getObjectBounds();
	std::vector<sf::FloatRect> ladderBottomBounds = this->ladderBottomLayer->getObjectBounds();

	this->player.setResolved(false);
	this->player.setCollisionWithLadder(false);
	this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::NONE);

	sf::FloatRect overlap;

	// collision detection with platform layer
	for (const sf::FloatRect& objectBound : objectBounds)
	{
		if (objectBound.intersects(playerBound, overlap))
		{
			if (!this->player.isResolved())
			{
				auto collisionNormal = sf::Vector2f(objectBound.left, objectBound.top)
					- sf::Vector2f(playerBound.left, playerBound.top);
				resolveCollision(overlap, collisionNormal);
				//SPDLOG_INFO("PLATFORM");
			}
		}
	}

	for (const sf::FloatRect& ladderBottomBound : ladderBottomBounds)
	{
		if (ladderBottomBound.intersects(playerBound, overlap))
		{
			// a letra aljan allsz, csak felfele mehetsz, nem kell resolve
			this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::UP);
			this->player.setCollisionWithLadder(true);
			//SPDLOG_INFO("BOTTOM");
		}
	}

	for (const sf::FloatRect& ladderBound : ladderBounds)
	{
		if (ladderBound.intersects(playerBound, overlap))
		{
			// a letra kozepen allsz, barmerre mehetsz, nem kell resolve
			this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::BOTH);
			this->player.setCollisionWithLadder(true);
			//SPDLOG_INFO("MID");
		}
	}

	// collision detection with ladderTop layer
	for (const sf::FloatRect& ladderTopBound : ladderTopBounds)
	{
		if (ladderTopBound.intersects(playerBound, overlap))
		{
			// a letra tetejen allsz, barmerre mehetsz, kell resolve ha felette vagy es nem nyomod lefele
			this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::BOTH);
			this->player.setCollisionWithLadder(true);

			if (this->player.getActualClimbingState() == PlayerActualClimbingState::NONE
				&& !this->player.isResolved())
			{
				auto collisionNormal = sf::Vector2f(ladderTopBound.left, ladderTopBound.top)
					- sf::Vector2f(playerBound.left, playerBound.top);
				resolveCollision(overlap, collisionNormal);
				this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::DOWN);
				this->player.setActualClimbingState(PlayerActualClimbingState::NONE);
				//SPDLOG_INFO("TOP");
			}
		}
	}

	if (!this->player.isCollidingWithLadder())
	{
		this->player.setActualClimbingState(PlayerActualClimbingState::NONE);
	}

	//SPDLOG_INFO("RRR {} {}", this->player.gravity, this->player.actualClimbingState);
}

void Game::resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
	this->player.setResolved(true);

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