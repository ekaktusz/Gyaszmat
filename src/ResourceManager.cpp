#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
	loadTextures();
	loadFonts();
	loadMaps();
	loadMusic();
	loadSounds();
}

void ResourceManager::loadFonts()
{
	std::filesystem::path fontPath = assetPath / "fonts";
	this->fontHolder.load(res::Font::Roboto, fontPath / "roboto" / "Roboto-Regular.ttf");
	this->fontHolder.load(res::Font::Pixel, fontPath / "pixel_art_font.ttf");
	this->fontHolder.load(res::Font::Upheaval, fontPath / "upheaval.ttf");
	this->fontHolder.load(res::Font::Fipps, fontPath / "fipps.otf");
}

void ResourceManager::loadMaps()
{
	std::filesystem::path mapPath = assetPath / "maps";
	this->mapHolder.load(res::Map::TestMap, mapPath / "platform.tmx");
}

void ResourceManager::loadSounds()
{
	std::filesystem::path soundPath = assetPath / "sounds";
	this->soundBufferHolder.load(res::Sound::Jump1, soundPath / "jump.wav");
	this->soundBufferHolder.load(res::Sound::Jump2, soundPath / "jump2.wav");
	this->soundBufferHolder.load(res::Sound::FootStepGrass, soundPath / "grass_footstep.wav");
}

void ResourceManager::loadMusic()
{
	std::filesystem::path musicPath = assetPath / "music";
	this->musicHolder.load(res::Music::LudumDare1, musicPath / "ludumdare38" / "track1.wav");
	this->musicHolder.load(res::Music::LudumDare2, musicPath / "ludumdare38" / "track2.wav");
	this->musicHolder.load(res::Music::LudumDare3, musicPath / "ludumdare38" / "track3.wav");
	this->musicHolder.load(res::Music::LudumDare4, musicPath / "ludumdare38" / "track4.wav");
	this->musicHolder.load(res::Music::LudumDare5, musicPath / "ludumdare38" / "track5.wav");
	this->musicHolder.load(res::Music::LudumDare6, musicPath / "ludumdare38" / "track6.wav");
	this->musicHolder.load(res::Music::LudumDare7, musicPath / "ludumdare38" / "track7.wav");
	this->musicHolder.load(res::Music::LudumDare8, musicPath / "ludumdare38" / "track8.wav");
	this->musicHolder.load(res::Music::LudumDare9, musicPath / "ludumdare38" / "track9.wav");
	this->musicHolder.load(res::Music::LudumDare10, musicPath / "ludumdare38" / "track10.wav");
}

void ResourceManager::loadTextures()
{
	std::filesystem::path texturePath = assetPath / "textures";
	this->textureHolder.load(res::Texture::PlayerIdle, texturePath / "owlet_monster" / "Owlet_Monster_Idle_4.png");
	this->textureHolder.load(res::Texture::PlayerRun, texturePath / "owlet_monster" / "Owlet_Monster_Run_6.png");
	this->textureHolder.load(res::Texture::PlayerJump, texturePath / "owlet_monster" / "Owlet_Monster_Jump_8.png");
	this->textureHolder.load(res::Texture::PlayerClimb, texturePath / "owlet_monster" / "Owlet_Monster_Climb_4.png");
	this->textureHolder.load(res::Texture::MenuBackgroundMountain, texturePath / "menu_background_mountain.jpg");
	this->textureHolder.load(res::Texture::MenuBackgroundForest, texturePath / "menu_background_forest.png");
	this->textureHolder.load(res::Texture::PauseBackground, texturePath / "pause_background.png");
	this->textureHolder.load(res::Texture::ParallaxMountain1, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-bg.png");
	this->textureHolder.load(res::Texture::ParallaxMountain5, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-foreground-trees.png");
	this->textureHolder.load(res::Texture::ParallaxMountain2, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-montain-far.png");
	this->textureHolder.load(res::Texture::ParallaxMountain3, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-mountains.png");
	this->textureHolder.load(res::Texture::ParallaxMountain4, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-trees.png");
	this->textureHolder.load(res::Texture::ParallaxForest1, texturePath / "parallax_backgrounds" / "forest_pack" / "01-sky.png");
	this->textureHolder.load(res::Texture::ParallaxForest2, texturePath / "parallax_backgrounds" / "forest_pack" / "02-cloud.png");
	this->textureHolder.load(res::Texture::ParallaxForest3, texturePath / "parallax_backgrounds" / "forest_pack" / "03-mountain2.png");
	this->textureHolder.load(res::Texture::ParallaxForest4, texturePath / "parallax_backgrounds" / "forest_pack" / "04-pine1.png");
	this->textureHolder.load(res::Texture::ParallaxForest5, texturePath / "parallax_backgrounds" / "forest_pack" / "05-pine2.png");
}

const sf::Texture& ResourceManager::getTexture(res::Texture id)
{
	return textureHolder.get(id);
}

const sf::Font& ResourceManager::getFont(res::Font id)
{
	return fontHolder.get(id);
}

const tmx::Map& ResourceManager::getMap(res::Map id)
{
	return mapHolder.get(id);
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(res::Sound id)
{
	return soundBufferHolder.get(id);
}


sf::Music& ResourceManager::getMusic(res::Music id)
{
	return musicHolder.get(id);
}
