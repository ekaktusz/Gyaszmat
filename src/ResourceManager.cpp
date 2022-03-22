#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	loadTextures();
	loadFonts();
}

void ResourceManager::loadFonts()
{
	std::filesystem::path fontPath = assetPath / "fonts";
	this->fontHolder.load(res::Font::Roboto, fontPath / "roboto" / "Roboto-Regular.ttf");
}

void ResourceManager::loadTextures()
{
	std::filesystem::path texturePath = assetPath / "textures";
	this->textureHolder.load(res::Texture::PlayerIdle, texturePath / "owlet_monster" / "Owlet_Monster_Idle_4.png");
	this->textureHolder.load(res::Texture::PlayerRun, texturePath / "owlet_monster" / "Owlet_Monster_Run_6.png");
	this->textureHolder.load(res::Texture::PlayerJump, texturePath / "owlet_monster" / "Owlet_Monster_Jump_8.png");
}

const sf::Texture& ResourceManager::getTexture(res::Texture id)
{
	return textureHolder.get(id);
}

const sf::Font& ResourceManager::getFont(res::Font id)
{
	return fontHolder.get(id);
}