#pragma once

#include "ResourceTypes.h"
#include "pch.h"

class MusicPlayer
{
public:
	MusicPlayer();

	void chooseTrack(res::Music musicId);
	void play();
	void pause();
	void stop();
	void setPlayingOffset(sf::Time time);

	void setLoop(bool loop);
	void setPitch(float pitch);
	void setVolume(float volume);

private:
	sf::Music* m_Music;
};