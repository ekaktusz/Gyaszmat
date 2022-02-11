#pragma once

#include "Enemy.h"
#include "Player.h"

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
	sf::Event event;

	sf::Clock clock;
	sf::Time deltaTime;

	void render();
	void update();
	void updateCollision();
	void processEvents();

public:
	Game();
	~Game();

	void run();
};