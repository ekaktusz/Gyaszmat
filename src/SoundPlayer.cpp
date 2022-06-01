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
	this->sounds.push_back(GSound(soundId, sf::Sound()));
	
	sf::Sound& sound = this->sounds.back().sound;

	sound.setBuffer(ResourceManager::getInstance().getSoundBuffer(soundId));
	sound.setPosition(position.x, position.y, 0.f);
	sound.setAttenuation(attenuation);
	sound.setMinDistance(minDistance3d);

	sound.play();
}

void SoundPlayer::pause()
{
	for (GSound gsound : this->sounds)
	{
		gsound.sound.pause();
	}
}

void SoundPlayer::resume()
{
	for (GSound gsound : this->sounds)
	{
		if (gsound.sound.getStatus() == sf::Sound::Paused) gsound.sound.play();
	}
}

void SoundPlayer::removeStoppedSounds()
{
	this->sounds.remove_if([](const GSound& gsound) { return gsound.sound.getStatus() == sf::Sound::Stopped; });
}

void SoundPlayer::removeSoundsById(res::Sound soundId)
{
	this->sounds.remove_if([&](const GSound& gsound) { return gsound.soundId == soundId; });
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

