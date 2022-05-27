#include "MusicPlayer.h"
#include "ResourceManager.h"

MusicPlayer::MusicPlayer()
{}

void MusicPlayer::chooseTrack(res::Music musicId)
{
	this->music = &ResourceManager::getInstance().getMusic(musicId);
}

void MusicPlayer::play()
{
	this->music->play();
}

void MusicPlayer::stop()
{
	this->music->stop();
}

void MusicPlayer::setPaused(bool paused)
{
	paused ? this->music->pause() : this->music->play();
}

void MusicPlayer::setVolume(float volume)
{
	this->music->setVolume(volume);
}
