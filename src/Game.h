#pragma once

#include "Enemy.h"
#include "Player.h"

class Game
{
private:
	static const unsigned int XX = 800;
	static const unsigned int YY = 600;
	static const std::string name;

	Player player;
	std::vector<Enemy> enemies;

	sf::RenderWindow renderWindow;
	sf::Event event;

	void render();
	void update();
	void updateCollision();

public:
	Game();
	~Game();

	void run();
};