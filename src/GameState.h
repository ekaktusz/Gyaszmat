#pragma once

#include "pch.h"
#include "HealthBar.h"
#include "State.h"
#include "ParallaxBackground.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"

class Player;
class TileLayer;
class LadderLayer;
class Terrain;

class GameState : public State
{
public:
	GameState(Game* game);
	~GameState();

	// Inherited via State
	virtual void render() override;
	virtual void update(sf::Time deltaTime) override;
	virtual void handleEvent(const sf::Event& event) override;

private:
	void updateCollision();

private:
	Player* m_Player;

	sf::View m_View;

	sf::Clock m_Clock;
	float m_FrameTime;

	tmx::FloatRect m_MapSize;
	const tmx::Map* m_Map;
	TileLayer* m_TileLayerFar;
	TileLayer* m_TileLayerMiddle;
	TileLayer* m_TileLayerNear;
	LadderLayer* m_LadderLayer;
	Terrain* m_Terrain;
	
	ParallaxBackground m_ParallaxBackground;

	HealthBar m_PlayerHealthBar;
	Label m_FrameTimeLabel;

	MusicPlayer m_MusicPlayer;
	SoundPlayer m_SoundPlayer;
};