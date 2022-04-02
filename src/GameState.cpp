#include "GameState.h"
#include "ResourceManager.h"
#include "PauseState.h"

GameState::GameState(Game* game)
{
	this->game = game;
	this->map = &ResourceManager::getInstance().getMap(res::Map::TestMap);
	this->tileLayerFar = new MapLayer(*this->map, 0);
	this->tileLayerMiddle = new MapLayer(*this->map, 1);
	this->tileLayerNear = new MapLayer(*this->map, 2);
	this->objectLayer = new MapLayer(*this->map, 3);
	this->ladderLayer = new MapLayer(*this->map, 4);
	this->ladderTopLayer = new MapLayer(*this->map, 5);
	this->ladderBottomLayer = new MapLayer(*this->map, 6);
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
	delete ladderLayer;
	delete ladderTopLayer;
	delete ladderBottomLayer;
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
	std::vector<sf::FloatRect> ladderBounds = this->ladderLayer->getObjectBounds();
	std::vector<sf::FloatRect> ladderTopBounds = this->ladderTopLayer->getObjectBounds();
	std::vector<sf::FloatRect> ladderBottomBounds = this->ladderBottomLayer->getObjectBounds();

	this->player.setResolved(false);
	this->player.setCollisionWithLadder(false);
	this->player.setPossibleClimbingDirections(PlayerPossibleClimbingDir::NONE);

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