#pragma once
#include "pch.h"
#include "ResourceHolder.h"
#include "ResourceTypes.h"

//template <typename Resource = sf::Texture, typename Identifier = res::Texture>
class TextureHolder : public ResourceHolder<sf::Texture, res::Texture>
{
public:
	void load(res::Texture id, const std::filesystem::path& filePath) override
	{
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(filePath.string()))
		{
			SPDLOG_ERROR("Failed to load " + filePath.string());
			return;
		}
		SPDLOG_INFO("SUCCESS READ:" + filePath.string());
		this->insert(id, texture);
	}
};
