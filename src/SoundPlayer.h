#pragma once

#include "pch.h"
#include "ResourceTypes.h"

class SoundPlayer
{
public:
	SoundPlayer();

	void play(res::Sound soundId);
	void play(res::Sound soundId, sf::Vector2f position);
	void pause();
	void resume();

	void removeStoppedSounds();
	void setListenerPosition(sf::Vector2f position);
	sf::Vector2f getListenerPosition() const;

private:
	std::list<sf::Sound> sounds; // use std::list instead of vector that it garuanntees no relocating, and removing is easier

};