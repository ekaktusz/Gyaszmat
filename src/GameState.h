#pragma once

#include "Enemy.h"
#include "HealthBar.h"
#include "MapLayer.h"
#include "Player.h"
#include "State.h"
#include "pch.h"

class GameState : public State
{
private:
	Player player;
	std::vector<Enemy> enemies;

	sf::View view;

	sf::Clock clock;

	const tmx::Map* map;
	MapLayer* tileLayerFar;
	MapLayer* tileLayerMiddle;
	MapLayer* tileLayerNear;
	MapLayer* objectLayer;

	HealthBar playerHealthBar;

	void render() override;
	void update(sf::Time deltaTime) override;
	void handleEvent(const sf::Event& event) override;

	void updateCollision();
	void resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

public:
	GameState(Game* game);
	~GameState();

	void run();
};