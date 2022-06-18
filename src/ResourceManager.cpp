#include "ResourceManager.h"

namespace 
{
	const std::filesystem::path assetPath = std::filesystem::current_path().parent_path() / "assets";
	const std::filesystem::path fontPath = assetPath / "fonts";
	const std::filesystem::path mapPath = assetPath / "maps";
	const std::filesystem::path soundPath = assetPath / "sounds";
	const std::filesystem::path musicPath = assetPath / "music";
	const std::filesystem::path texturePath = assetPath / "textures";
}

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
	m_FontHolder.load(res::Font::Roboto, fontPath / "roboto" / "Roboto-Regular.ttf");
	m_FontHolder.load(res::Font::Pixel, fontPath / "pixel_art_font.ttf");
	m_FontHolder.load(res::Font::Upheaval, fontPath / "upheaval.ttf");
	m_FontHolder.load(res::Font::Fipps, fontPath / "fipps.otf");
}

void ResourceManager::loadMaps()
{
	m_MapHolder.load(res::Map::TestMap, mapPath / "platform.tmx");
}

void ResourceManager::loadSounds()
{
	m_SoundBufferHolder.load(res::Sound::Jump1, soundPath / "jump.wav");
	m_SoundBufferHolder.load(res::Sound::Jump2, soundPath / "jump2.wav");
	m_SoundBufferHolder.load(res::Sound::FootStepGrass, soundPath / "grass_footstep.wav");
}

void ResourceManager::loadMusic()
{
	m_MusicHolder.load(res::Music::LudumDare1, musicPath / "ludumdare38" / "track1.wav");
	m_MusicHolder.load(res::Music::LudumDare2, musicPath / "ludumdare38" / "track2.wav");
	m_MusicHolder.load(res::Music::LudumDare3, musicPath / "ludumdare38" / "track3.wav");
	m_MusicHolder.load(res::Music::LudumDare4, musicPath / "ludumdare38" / "track4.wav");
	m_MusicHolder.load(res::Music::LudumDare5, musicPath / "ludumdare38" / "track5.wav");
	m_MusicHolder.load(res::Music::LudumDare6, musicPath / "ludumdare38" / "track6.wav");
	m_MusicHolder.load(res::Music::LudumDare7, musicPath / "ludumdare38" / "track7.wav");
	m_MusicHolder.load(res::Music::LudumDare8, musicPath / "ludumdare38" / "track8.wav");
	m_MusicHolder.load(res::Music::LudumDare9, musicPath / "ludumdare38" / "track9.wav");
	m_MusicHolder.load(res::Music::LudumDare10, musicPath / "ludumdare38" / "track10.wav");
}

void ResourceManager::loadTextures()
{
	m_TextureHolder.load(res::Texture::PlayerIdle, texturePath / "owlet_monster" / "Owlet_Monster_Idle_4.png");
	m_TextureHolder.load(res::Texture::PlayerRun, texturePath / "owlet_monster" / "Owlet_Monster_Run_6.png");
	m_TextureHolder.load(res::Texture::PlayerJump, texturePath / "owlet_monster" / "Owlet_Monster_Jump_8.png");
	m_TextureHolder.load(res::Texture::PlayerClimb, texturePath / "owlet_monster" / "Owlet_Monster_Climb_4.png");
	m_TextureHolder.load(res::Texture::MenuBackgroundMountain, texturePath / "menu_background_mountain.jpg");
	m_TextureHolder.load(res::Texture::MenuBackgroundForest, texturePath / "menu_background_forest.png");
	m_TextureHolder.load(res::Texture::PauseBackground, texturePath / "pause_background.png");
	m_TextureHolder.load(res::Texture::ParallaxMountain1, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-bg.png");
	m_TextureHolder.load(res::Texture::ParallaxMountain5, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-foreground-trees.png");
	m_TextureHolder.load(res::Texture::ParallaxMountain2, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-montain-far.png");
	m_TextureHolder.load(res::Texture::ParallaxMountain3, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-mountains.png");
	m_TextureHolder.load(res::Texture::ParallaxMountain4, texturePath / "parallax_backgrounds" / "mountain_pack" / "parallax-mountain-trees.png");
	m_TextureHolder.load(res::Texture::ParallaxForest1, texturePath / "parallax_backgrounds" / "forest_pack" / "01-sky.png");
	m_TextureHolder.load(res::Texture::ParallaxForest2, texturePath / "parallax_backgrounds" / "forest_pack" / "02-cloud.png");
	m_TextureHolder.load(res::Texture::ParallaxForest3, texturePath / "parallax_backgrounds" / "forest_pack" / "03-mountain2.png");
	m_TextureHolder.load(res::Texture::ParallaxForest4, texturePath / "parallax_backgrounds" / "forest_pack" / "04-pine1.png");
	m_TextureHolder.load(res::Texture::ParallaxForest5, texturePath / "parallax_backgrounds" / "forest_pack" / "05-pine2.png");
}

const sf::Texture& ResourceManager::getTexture(res::Texture id)
{
	return m_TextureHolder.get(id);
}

const sf::Font& ResourceManager::getFont(res::Font id)
{
	return m_FontHolder.get(id);
}

const tmx::Map& ResourceManager::getMap(res::Map id)
{
	return m_MapHolder.get(id);
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(res::Sound id)
{
	return m_SoundBufferHolder.get(id);
}


sf::Music& ResourceManager::getMusic(res::Music id)
{
	return m_MusicHolder.get(id);
}
