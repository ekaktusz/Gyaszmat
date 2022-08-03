#pragma once

#include "pch.h"
#include "HealthBar.h"
#include "State.h"
#include "ParallaxBackground.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"
#include "CsabRect.h"

class Player;
class TileLayer;
class LadderLayer;
class TerrainLayer;

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
	TerrainLayer* m_Terrain;

	ParallaxBackground m_ParallaxBackground;

	HealthBar m_PlayerHealthBar;
	Label m_FrameTimeLabel;

	MusicPlayer m_MusicPlayer;
	SoundPlayer m_SoundPlayer;


	//FUN
	b2World* world;
	b2BodyDef groundBodyDef;
	b2Body* groundBody;
	sf::RectangleShape groundBodySFMLShape;
	b2PolygonShape groundBox;

	b2BodyDef bodyDef;
	b2Body* body;
	sf::RectangleShape bodySFMLShape;
	b2PolygonShape dynamicBox;

	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	CsabRect* csabrect1;
	CsabRect* csabrect2;

};