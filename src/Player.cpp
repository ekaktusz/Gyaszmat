#include "Player.h"

Player::Player()
{
    texture.loadFromFile("./assets/test_player.png");
    sprite.setTexture(texture); 
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite);
}