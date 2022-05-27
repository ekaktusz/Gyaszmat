#pragma once
#include "ResourceTypes.h"
#include "pch.h"

class MusicPlayer
{
public:
	MusicPlayer();

	void chooseTrack(res::Music musicId);
	void play();
	void stop();

	void setPaused(bool paused);
	void setVolume(float volume);

private:
	sf::Music* music;
};