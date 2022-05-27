#pragma once
#include "pch.h"
#include "ResourceTypes.h"
#include "SoundBufferHolder.h"

class SoundPlayer
{
public:
	SoundPlayer();

	void play(res::Sound soundId);

	void removeStoppedSounds();
	void setListenerPosition(sf::Vector2f position);
	sf::Vector2f getListenerPosition() const;

private:
	SoundBufferHolder soundBufferHolder;
	std::list<sf::Sound> sounds; // use std::list instead of vector that it garuanntees no relocating, and removing is easier

};