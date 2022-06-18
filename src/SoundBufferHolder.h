#pragma once
#include "ResourceHolder.h"
#include "ResourceTypes.h"
#include "pch.h"

class SoundBufferHolder : public ResourceHolder<sf::SoundBuffer, res::Sound>
{
public:
	void load(res::Sound id, const std::filesystem::path& filePath) override
	{
		std::unique_ptr<sf::SoundBuffer> sound = std::make_unique<sf::SoundBuffer>();
		if (!sound->loadFromFile(filePath.string()))
		{
			SPDLOG_ERROR("Failed to parse tiled map: " + filePath.string());
			return;
		}
		SPDLOG_INFO("Sound loaded succesfully: " + filePath.string());
		insert(id, std::move(sound));
	}
};
