#pragma once
#include "pch.h"
#include "ResourceHolder.h"
#include "ResourceTypes.h"

class TextureHolder : public ResourceHolder<sf::Texture, res::Texture>
{
public:
	void load(res::Texture id, const std::filesystem::path& filePath) override
	{
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(filePath.string()))
		{
			SPDLOG_ERROR("Failed to load texture: " + filePath.string());
			return;
		}
		SPDLOG_INFO("Texture loaded succesfully: " + filePath.string());
		this->insert(id, texture);
	}
};
