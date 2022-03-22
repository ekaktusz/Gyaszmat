#pragma once

#include "pch.h"
#include "TextureHolder.h"
#include "FontHolder.h"
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

	const sf::Texture& getTexture(res::Texture id);
	const sf::Font& getFont(res::Font id);

private:
	ResourceManager();

	void loadTextures();
	void loadFonts();

	// Holders for each type
	TextureHolder textureHolder;
	FontHolder fontHolder;

	std::filesystem::path assetPath = std::filesystem::current_path().parent_path() / "assets";
};


