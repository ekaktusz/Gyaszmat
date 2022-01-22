#pragma once

#include <cstdlib>

/*
#include <ctime>
#include <iostream>
#include <time.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
*/

#include "Player.h"
#include "Enemy.h"
#include <SFML/Graphics.hpp>

class Game
{
private:
    Player player; //later if we have multiple players change it to vector?
    std::vector<Enemy> enemies;
    sf::RenderWindow renderWindow;

    void draw();
public:
    static const unsigned int XX = 800;
    static const unsigned int YY = 600;
    static const std::string name;

    Game(/* args */);
    ~Game();

    void run();
};