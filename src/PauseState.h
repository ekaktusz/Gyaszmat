#pragma once
#include "pch.h"
#include "State.h"
#include "Label.h"
#include "Button.h"
#include "MusicPlayer.h"

class PauseState : public State
{
public:
	PauseState(Game* game);

	// Inherited via State
	virtual void render() override;
	virtual void update(sf::Time deltaTime) override;
	virtual void handleEvent(const sf::Event& event) override;

private:
	sf::View view;

	sf::Texture backgroundTexture;
	sf::Sprite background;

	Button continueButton;
	Button mainMenuButton;

	MusicPlayer musicPlayer;

	Label titleLabel;
};
