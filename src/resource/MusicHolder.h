#pragma once

#include "ResourceHolder.h"
#include "ResourceTypes.h"
#include "pch.h"

class MusicHolder : public ResourceHolder<sf::Music, res::Music>
{
public:
	void load(res::Music id, const std::filesystem::path& filePath) override
	{
		std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
		if (!music->openFromFile(filePath.string()))
		{
			SPDLOG_ERROR("Failed to load music: " + filePath.string());
			return;
		}
		SPDLOG_INFO("Music loaded successfully: " + filePath.string());
		insert(id, std::move(music));
	}
};
