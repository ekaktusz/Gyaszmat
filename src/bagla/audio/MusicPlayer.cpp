#include "MusicPlayer.h"
#include "ResourceManager.h"

MusicPlayer::MusicPlayer() : m_Music(nullptr)
{}

void MusicPlayer::chooseTrack(res::Music musicId)
{
	m_Music = &ResourceManager::getInstance().getMusic(musicId);
}

void MusicPlayer::play()
{
	if (m_Music->getStatus() != sf::SoundSource::Playing) m_Music->play();
}

void MusicPlayer::pause()
{
	m_Music->pause();
}

void MusicPlayer::stop()
{
	m_Music->stop();
}

void MusicPlayer::setPlayingOffset(sf::Time time)
{
	m_Music->setPlayingOffset(time);
}

void MusicPlayer::setLoop(bool loop)
{
	m_Music->setLoop(loop);
}

void MusicPlayer::setPitch(float pitch)
{
	m_Music->setPitch(pitch);
}

// between 0 and 100
void MusicPlayer::setVolume(float volume)
{
	m_Music->setVolume(volume);
}
