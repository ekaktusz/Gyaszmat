#include "Game.h"

Game::Game() : renderWindow({ Game::XX, Game::YY }, Game::name)
{
	this->map.load(
		(std::filesystem::current_path().parent_path() / "assets" / "platform.tmx").string());
	this->tileLayer = new MapLayer(map, 1);
	this->objectLayer = new MapLayer(map, 3);
	this->view = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(Game::XX, Game::YY));
	this->renderWindow.setFramerateLimit(Game::FPS);
}

Game::~Game()
{
	delete tileLayer;
	delete objectLayer;
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
	this->renderWindow.draw(*this->tileLayer);
	this->renderWindow.draw(this->player);
	this->view.setCenter(this->player.getCenterPosition());
	this->renderWindow.setView(this->view);
	this->renderWindow.display();
}

void Game::updateCollision()
{
	sf::FloatRect playerBounds = this->player.getGlobalBounds();
	std::vector<sf::FloatRect> objectBounds = this->objectLayer->getObjectBounds();

	// collision detection with every object on object layer
	sf::FloatRect overlap;
	for (const sf::FloatRect& objectBound : objectBounds)
	{
		if (objectBound.intersects(this->player.getGlobalBounds(), overlap))
		{
			this->player.stopFalling();
			//auto collisionNormal = o.getPosition() - getPosition();
			auto collisionNormal =
				sf::Vector2f(objectBound.left, objectBound.top) - this->player.getCenterPosition();
			auto manifold = getManifold(overlap, collisionNormal);
			resolve(manifold);
		}
	}
}

void Game::resolve(const sf::Vector3f& manifold)
{
	sf::Vector2f normal(manifold.x, manifold.y);
	this->player.move(normal * manifold.z);
}

sf::Vector3f Game::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
	//the collision normal is stored in x and y, with the penetration in z
	sf::Vector3f manifold;

	if (overlap.width < overlap.height)
	{
		manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
		manifold.z = overlap.width;
	}
	else
	{
		manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
		manifold.z = overlap.height;
	}

	return manifold;
}