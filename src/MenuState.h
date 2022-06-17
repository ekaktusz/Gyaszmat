#pragma once

#include "pch.h"
#include "State.h"
#include "Button.h"
#include "MusicPlayer.h"
#include "SoundPlayer.h"

class MenuState : public State
{
public:
	MenuState(Game* game);

	// Inherited via State
	virtual void render() override;
	virtual void update(sf::Time deltaTime) override;
	virtual void handleEvent(const sf::Event& event) override;

private:
	sf::View m_View;

	sf::Texture m_BackgroundTexture;
	sf::Sprite m_Background;

	Button m_StartButton;
	Button m_ExitButton;

	MusicPlayer m_MusicPlayer;
	SoundPlayer m_SoundPlayer; // Will be used for button sounds etc.

	Label m_TitleLabel;
};