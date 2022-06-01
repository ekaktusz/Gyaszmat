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
	if (this->music->getStatus() != sf::SoundSource::Playing) this->music->play();
}

void MusicPlayer::pause()
{
	this->music->pause();
}

void MusicPlayer::stop()
{
	this->music->stop();
}

void MusicPlayer::setPlayingOffset(sf::Time time)
{
	this->music->setPlayingOffset(time);
}

void MusicPlayer::setLoop(bool loop)
{
	this->music->setLoop(loop);
}

void MusicPlayer::setPitch(float pitch)
{
	this->music->setPitch(pitch);
}

// between 0 and 100
void MusicPlayer::setVolume(float volume)
{
	this->music->setVolume(volume);
}
