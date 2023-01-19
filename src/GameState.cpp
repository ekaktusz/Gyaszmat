#include "GameState.h"
#include "MapLayerNames.h"
#include "PauseState.h"
#include "ResourceManager.h"

#include "Player.h"
#include "TileLayer.h"
#include "LadderLayer.h"
#include "TerrainLayer.h"

GameState::GameState(Game* game) : State(game), m_Player(new Player(m_SoundPlayer)), m_FrameTime(0.f)
{
	// Init m_Map
	m_Map = &ResourceManager::getInstance().getMap(res::Map::TestMap);
	m_TileLayerFar = new TileLayer(*m_Map, MapLayerNames::TileLayerName::BackLayer);
	m_TileLayerMiddle = new TileLayer(*m_Map, MapLayerNames::TileLayerName::MidLayer);
	m_TileLayerNear = new TileLayer(*m_Map, MapLayerNames::TileLayerName::FrontLayer);
	m_LadderLayer = new LadderLayer(*m_Map);
	m_TerrainLayer = new TerrainLayer(*m_Map);
	m_MapSize = m_Map->getBounds();

	m_View = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(Game::s_WindowSizeX, Game::s_WindowSizeY));
	m_Game->renderWindow.setView(m_View);

	// Init healthbar
	m_PlayerHealthBar.setHealth(100);
	m_PlayerHealthBar.setMaxHealth(100);

	// Init background
	m_ParallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxForest1), 1.f));
	m_ParallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxForest2), 0.97f));
	m_ParallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxForest3), 0.94f, 30.f));
	m_ParallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxForest4), 0.85f, 50.f));
	m_ParallaxBackground.addLayer(new ParallaxLayer(
		ResourceManager::getInstance().getTexture(res::Texture::ParallaxForest5), 0.8f, 50.f));

	m_ParallaxBackground.setScale(Game::s_WindowSizeY / m_ParallaxBackground.getGlobalBounds().height * 1.5,
		Game::s_WindowSizeY / m_ParallaxBackground.getGlobalBounds().height * 1.5);

	// Init frame time widget
	m_FrameTimeLabel.getText().setFont(ResourceManager::getInstance().getFont(res::Font::Roboto));
	m_FrameTimeLabel.getText().setString("HLLO");
	m_FrameTimeLabel.getText().setCharacterSize(30);
	m_FrameTimeLabel.getText().setFillColor(sf::Color::Yellow);
	m_FrameTimeLabel.getText().setOutlineColor(sf::Color::Black);
	m_FrameTimeLabel.getText().setOutlineColor(sf::Color::Black);
	m_FrameTimeLabel.getText().setOutlineThickness(1.f);

	// Init background music
	m_MusicPlayer.chooseTrack(res::Music::LudumDare2);
	m_MusicPlayer.play();
	m_MusicPlayer.setVolume(100);
	m_MusicPlayer.setLoop(true);

	// Light
	light.setRange(600);
	light.setIntensity(1);
	fog.setAreaColor(sf::Color::Black);
	fog.setAreaOpacity(.9);
}

GameState::~GameState()
{
	delete m_TileLayerFar;
	delete m_TileLayerMiddle;
	delete m_TileLayerNear;
	delete m_LadderLayer;
	delete m_TerrainLayer;
	delete m_Player;
}

void GameState::update(sf::Time deltaTime)
{
	m_SoundPlayer.removeStoppedSounds();
	m_MusicPlayer.play();
	m_Player->update();
	const sf::Vector2f movement = m_Player->getCenterPosition() - m_View.getCenter() - sf::Vector2f(0.f, Game::s_WindowSizeY / 8);
	m_View.move(movement * deltaTime.asSeconds() * 10.f);
	updateCollision();
	m_PlayerHealthBar.update(m_Player->getHealth());

	m_CameraPosition = { m_View.getCenter()
		- sf::Vector2f(Game::s_WindowSizeX / 2, Game::s_WindowSizeY / 2) };
	m_PlayerHealthBar.setPosition(m_CameraPosition);
	m_FrameTimeLabel.getText().setPosition(m_CameraPosition);
	
	// Change to 1.f / this->m_FrameTime to show FPS
	m_FrameTimeLabel.getText().setString(std::to_string(m_FrameTime));
	m_ParallaxBackground.update(m_CameraPosition);

	updateLights();
}

void GameState::updateLights()
{
	fog.setPosition(m_CameraPosition);
	light.setPosition(m_Player->getCenterPosition());
	fog.clear();
	fog.draw(light);
	fog.display();
}

void GameState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::Closed)
	{
		m_Game->renderWindow.close();
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			SPDLOG_INFO("Switch to PauseState...");
			m_Player->stop();
			m_MusicPlayer.pause();
			m_Game->pushState(new PauseState(m_Game));
		}
	}
	m_Player->handleEvent(event);
}

void GameState::render()
{
	m_FrameTime = 1 / m_Clock.restart().asSeconds();
	m_Game->renderWindow.clear();
	m_Game->renderWindow.setView(m_View);
	m_Game->renderWindow.draw(m_ParallaxBackground);
	m_Game->renderWindow.draw(*m_TileLayerFar); // layer behind m_Player
	m_Game->renderWindow.draw(*m_TileLayerMiddle); // layer of m_Map
	m_Game->renderWindow.draw(*m_Player);
	m_Game->renderWindow.draw(*m_TileLayerNear); // layer before m_Player
	m_Game->renderWindow.draw(fog);
	m_Game->renderWindow.draw(m_PlayerHealthBar);
	m_Game->renderWindow.draw(m_FrameTimeLabel); // comment out if dont want to see frame
	m_Game->renderWindow.display();
}

void GameState::updateCollision()
{
	m_Player->setResolved(false);
	m_TerrainLayer->updateCollision(*m_Player);
	m_LadderLayer->updateCollision(*m_Player);
}