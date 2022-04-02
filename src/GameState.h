#pragma once

#include "Enemy.h"
#include "HealthBar.h"
#include "TileLayer.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "State.h"
#include "pch.h"
#include "Ladder.h"

class GameState : public State
{
private:
	Player player;
	std::vector<Enemy> enemies;

	sf::View view;

	sf::Clock clock;

	const tmx::Map* map;
	TileLayer* tileLayerFar;
	TileLayer* tileLayerMiddle;
	TileLayer* tileLayerNear;
	ObjectLayer* objectLayer;
	Ladder* ladder;

	HealthBar playerHealthBar;

	void render() override;
	void update(sf::Time deltaTime) override;
	void handleEvent(const sf::Event& event) override;

	void updateCollision();
	void resolveCollision(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal);

public:
	GameState(Game* game);
	~GameState();
};