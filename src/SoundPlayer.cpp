#include "pch.h"
#include "SoundPlayer.h"
#include "ResourceManager.h"

SoundPlayer::SoundPlayer()
{
	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(res::Sound soundId)
{
	this->sounds.push_back(sf::Sound(ResourceManager::getInstance().getSoundBuffer(soundId)));
	this->sounds.back().play();
}

void SoundPlayer::removeStoppedSounds()
{
	sounds.remove_if([](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}

/* void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	//sf::Listener::setPosition(position.x, -position.y);
}*/
