#pragma once

#include "Enemy.h"
#include "HealthBar.h"
#include "MapLayer.h"
#include "Player.h"

class Game
{
private:
	static const unsigned int XX = 800;
	static const unsigned int YY = 600;
	const std::string name = "Gyaszmat";
	static const unsigned int MAX_FPS = 60;

	Player player;
	std::vector<Enemy> enemies;

	sf::RenderWindow renderWindow;
	sf::View view;
	sf::Event event;

	sf::Clock clock;
	float deltaTime;
	int currentFPS;

	tmx::Map map;
	MapLayer* tileLayerFar;
	MapLayer* tileLayerMiddle;
	MapLayer* tileLayerNear;
	MapLayer* objectLayer;

	HealthBar* playerHealthBar;


	void render();
	void update();
	void updateCollision();
	void processEvents();
	void resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

public:
	Game();
	~Game();

	void run();
};