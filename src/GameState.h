#pragma once

#include "pch.h"
#include "HealthBar.h"
#include "State.h"
#include "ParallaxBackground.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"

class Player;
class TileLayer;
class Ladder;
class Terrain;

class GameState : public State
{
public:
	GameState(Game* game);
	~GameState();

	// Inherited via State
	void render() override;
	void update(sf::Time deltaTime) override;
	void handleEvent(const sf::Event& event) override;

private:
	Player* player;

	sf::View view;

	sf::Clock clock;
	float frameTime;

	tmx::FloatRect mapSize;

	const tmx::Map* map;
	TileLayer* tileLayerFar;
	TileLayer* tileLayerMiddle;
	TileLayer* tileLayerNear;
	Ladder* ladder;
	Terrain* terrain;
	
	ParallaxBackground parallaxBackground;

	HealthBar playerHealthBar;
	Label frameTimeLabel;

	MusicPlayer musicPlayer;
	SoundPlayer soundPlayer;

	void updateCollision();
};