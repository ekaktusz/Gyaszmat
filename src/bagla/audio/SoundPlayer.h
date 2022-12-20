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
	void removeSoundsById(res::Sound soundId);
	void setListenerPosition(sf::Vector2f position);
	sf::Vector2f getListenerPosition() const;

private:
	struct GSound
	{
		res::Sound soundId;
		sf::Sound sound;
		GSound(res::Sound soundId, const sf::Sound& sound) : soundId(soundId), sound(sound)
		{}
	};
	std::list<GSound> m_Sounds; // use std::list instead of vector that it garuanntees no relocating, and removing is easier

};