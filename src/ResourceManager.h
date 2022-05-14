#pragma once

#include "pch.h"
#include "TextureHolder.h"
#include "FontHolder.h"
#include "MapHolder.h"
#include "SoundBufferHolder.h"
#include "MusicHolder.h"

// Singleton for accessing resources in all classes. Loads everything in it's constructor, in the first time it accessed.
// Everytime we want to add a new asset we have to append it's methods.

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
	const sf::SoundBuffer& getSoundBuffer(res::Sound id);
	const sf::Music& getMusic(res::Music id);

private:
	ResourceManager();

	void loadTextures();
	void loadFonts();
	void loadMaps();
	void loadSounds();
	void loadMusic();

	// Holders for each type
	TextureHolder textureHolder;
	FontHolder fontHolder;
	MapHolder mapHolder;
	MusicHolder musicHolder;
	SoundBufferHolder soundBufferHolder;

	std::filesystem::path assetPath = std::filesystem::current_path().parent_path() / "assets";
};


