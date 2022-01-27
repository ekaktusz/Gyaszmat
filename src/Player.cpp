#include "Player.h"

#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

Player::Player()
{
    string currentWorkingDirectory;
    char buffer[1024];
    char *answer = getcwd(buffer, sizeof(buffer));

    if (answer)
    {
        currentWorkingDirectory = answer;
    }

    texture.loadFromFile(currentWorkingDirectory + "/assets/test_player.png");
    sprite.setTexture(texture);
}

Player::~Player()
{
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(sprite);
}