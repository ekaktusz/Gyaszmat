#pragma once

#include "Enemy.h"
#include "Player.h"
#include "MapLayer.h"

class Game
{
private:
	static const unsigned int XX = 800;
	static const unsigned int YY = 600;
	const std::string name = "Gyaszmat";
	static const unsigned int FPS = 60;

	Player player;
	std::vector<Enemy> enemies;

	sf::RenderWindow renderWindow;
	sf::View view;
	sf::Event event;

	sf::Clock clock;
	sf::Time deltaTime;

	tmx::Map map;
	MapLayer* tileLayerFar;
	MapLayer* tileLayerMiddle;
	MapLayer* tileLayerNear;
	MapLayer* objectLayer;

	void render();
	int update();
	void updateCollision();
	int processEvents();
	void resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

public:
	Game();
	~Game();

	int run();
};