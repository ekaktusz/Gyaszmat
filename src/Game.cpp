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
	this->renderWindow.setView(this->view);
	this->playerHealthBar = new HealthBar(100, 100);
}

Game::~Game()
{
	delete tileLayerFar;
	delete tileLayerMiddle;
	delete tileLayerNear;
	delete objectLayer;
	delete playerHealthBar;
	delete ladderLayer;
	delete ladderTopLayer;
	delete ladderBottomLayer;
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
	this->playerHealthBar->update(this->player.getHealth());
	this->playerHealthBar->setPosition(
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
	this->renderWindow.draw(*this->playerHealthBar);
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
			sf::Vector2<float> collisionNormal = sf::Vector2f(objectBound.left, objectBound.top)
				- sf::Vector2f(playerBound.left, playerBound.top);
			resolveCollision(overlap, collisionNormal);
		}
	}

	for (const sf::FloatRect& ladderBottomBound : ladderBottomBounds)
	{
		if (ladderBottomBound.intersects(playerBound, overlap))
		{
			// a letra aljan allsz, csak felfele mehetsz, nem kell resolve
			this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::UP);
			this->player.setCollisionWithLadder(true);
		}
	}

	for (const sf::FloatRect& ladderBound : ladderBounds)
	{
		if (ladderBound.intersects(playerBound, overlap))
		{
			// a letra kozepen allsz, barmerre mehetsz, nem kell resolve
			this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::BOTH);
			this->player.setCollisionWithLadder(true);
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
				sf::Vector2<float> collisionNormal =
					sf::Vector2f(ladderTopBound.left, ladderTopBound.top)
					- sf::Vector2f(playerBound.left, playerBound.top);
				resolveCollision(overlap, collisionNormal);
				this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::DOWN);
				this->player.setActualClimbingState(PlayerActualClimbingState::NONE);
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