#pragma once

#include "pch.h"
#include "TextureHolder.h"
#include "MapLayer.h"

class ResourceManager
{
public:

	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	}
	
	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	void loadAssets();

	const ResourceHolder<sf::Texture, res::Texture>& getTextureHolder() const;

private:
	ResourceManager();

	void loadTextures();

	// Holders for each type
	TextureHolder textureHolder;

	std::filesystem::path assetPath = std::filesystem::current_path().parent_path() / "assets";
};


