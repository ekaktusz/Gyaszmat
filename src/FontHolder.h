#pragma once
#include "ResourceHolder.h"
#include "ResourceTypes.h"
#include "pch.h"

class FontHolder : public ResourceHolder<sf::Font, res::Font>
{
public:
	void load(res::Font id, const std::filesystem::path& filePath) override
	{
		std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>();
		if (!font->loadFromFile(filePath.string()))
		{
			SPDLOG_ERROR("Failed to load font: " + filePath.string());
			return;
		}
		SPDLOG_INFO("Font loaded successfully: " + filePath.string());
		this->insert(id, std::move(font));
	}
};
