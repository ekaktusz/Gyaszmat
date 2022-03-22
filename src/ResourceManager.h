#pragma once

#include "pch.h"
#include "ResourceHolder.h"
#include "MapLayer.h"


namespace res
{
	enum class Texture
	{
		PlayerIdle,
		PlayerRun,
		PlayerJump
	};

	enum class Font
	{
		Roboto
	};

	enum class Music
	{
		PeacefulGameplay
	};

	enum class MapLayer
	{
		TerrainObjectLayer,
		TileLayerFar,
		tileLayerMiddle,
		tileLayerNear
	};
}


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
	const ResourceHolder<sf::Font, res::Font>& getFontHolder() const;

private:
	ResourceManager();

	void loadFonts();
	void loadTextures();

	// Holders for each type
	ResourceHolder<sf::Texture, res::Texture> textureHolder;
	ResourceHolder<sf::Font, res::Font> fontHolder;

	std::filesystem::path assetPath = std::filesystem::current_path().parent_path() / "assets";
};


