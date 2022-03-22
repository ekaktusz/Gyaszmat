#pragma once

#include "pch.h"
#include "TextureHolder.h"
#include "FontHolder.h"
#include "MapHolder.h"
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
	const tmx::Map& getMap(res::Map id);

private:
	ResourceManager();

	void loadTextures();
	void loadFonts();
	void loadMaps();

	// Holders for each type
	TextureHolder textureHolder;
	FontHolder fontHolder;
	MapHolder mapHolder;

	std::filesystem::path assetPath = std::filesystem::current_path().parent_path() / "assets";
};


