#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	loadAssets();
}

void ResourceManager::loadAssets()
{
	loadTextures();
}

void ResourceManager::loadTextures()
{
	std::filesystem::path texturePath = assetPath / "textures";
	this->textureHolder.load(res::Texture::PlayerIdle, texturePath / "owlet_monster" / "Owlet_Monster_Idle_4.png");
	this->textureHolder.load(res::Texture::PlayerRun, texturePath / "owlet_monster" / "Owlet_Monster_Run_6.png");
	this->textureHolder.load(res::Texture::PlayerJump, texturePath / "owlet_monster" / "Owlet_Monster_Jump_8.png");
}

const ResourceHolder<sf::Texture, res::Texture>& ResourceManager::getTextureHolder() const
{
	return textureHolder;
}