#include "Player.h"
#include <filesystem>

Player::Player()
{
    std::filesystem::path playerAsset = std::filesystem::current_path() / "assets" / "test_player.png";
    texture.loadFromFile(playerAsset.string());
    sprite.setTexture(texture); 
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}