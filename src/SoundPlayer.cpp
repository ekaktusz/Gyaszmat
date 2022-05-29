#include "pch.h"
#include "SoundPlayer.h"
#include "ResourceManager.h"

namespace
{
	const float listenerZ = 300.f;
	const float attenuation = 8.f;
	const float minDistance2d = 200.f;
	const float minDistance3d = std::sqrt(minDistance2d * minDistance2d + listenerZ * listenerZ);
}

SoundPlayer::SoundPlayer() : sounds()
{
	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(res::Sound soundId)
{
	play(soundId, getListenerPosition());
}

void SoundPlayer::play(res::Sound soundId, sf::Vector2f position)
{
	SPDLOG_INFO(this->sounds.empty());
	SPDLOG_INFO(this->sounds.size());
	this->sounds.clear();
	SPDLOG_INFO(this->sounds.size());
	this->sounds.push_back(sf::Sound());
	
	sf::Sound& sound = this->sounds.back();

	sound.setBuffer(ResourceManager::getInstance().getSoundBuffer(soundId));
	sound.setPosition(position.x, position.y, 0.f);
	sound.setAttenuation(attenuation);
	sound.setMinDistance(minDistance3d);

	sound.play();
}

void SoundPlayer::pause()
{
	for (sf::Sound sound : this->sounds)
	{
		sound.pause();
	}
}

void SoundPlayer::resume()
{
	for (sf::Sound& sound : this->sounds)
	{
		if (sound.getStatus() == sf::Sound::Paused) sound.play();
	}
}

void SoundPlayer::removeStoppedSounds()
{
	//sounds.remove_if([](const sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; });

	this->sounds.erase(
    std::remove_if(
        this->sounds.begin(), 
        this->sounds.end(),
        [](sf::Sound& s) { return s.getStatus() == sf::Sound::Stopped; }
    ), 
    this->sounds.end()
); 
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return sf::Vector2f(position.x, -position.y);
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
	sf::Listener::setPosition(position.x, -position.y, listenerZ);
}


