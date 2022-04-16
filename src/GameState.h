#pragma once

#include "Enemy.h"
#include "HealthBar.h"
#include "Ladder.h"
#include "ObjectLayer.h"
#include "Player.h"
#include "State.h"
#include "Terrain.h"
#include "TileLayer.h"
#include "pch.h"

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
	Ladder* ladder;
	Terrain* terrain;

	HealthBar playerHealthBar;

	void render() override;
	void update(sf::Time deltaTime) override;
	void handleEvent(const sf::Event& event) override;

	void updateCollision();

public:
	GameState(Game* game);
	~GameState();
};