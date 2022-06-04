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
	void render() override;
	void update(sf::Time deltaTime) override;
	void handleEvent(const sf::Event& event) override;

private:
	sf::View view;

	sf::Texture backgroundTexture;
	sf::Sprite background;

	Button startButton;
	Button exitButton;

	MusicPlayer musicPlayer;
	SoundPlayer soundPlayer; // Will be used for button sounds etc.

	Label titleLabel;
};