#include "Game.h"
#include <SFML/Window.hpp>
#include <iostream>

using namespace std;

const std::string Game::name = "Gyaszmat";

Game::Game() : renderWindow({Game::XX, Game::YY}, Game::name)
{
}

Game::~Game()
{
}

void Game::run()
{
    while (renderWindow.isOpen())
    {
        sf::Event event;
        while (renderWindow.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                renderWindow.close();
                break;

            case sf::Event::KeyPressed:

                if (event.key.code == sf::Keyboard::W)
                {
                    sf::Vector2f position = player.sprite.getPosition();
                    //cout << "WWW: x: " << position.x << ", y: " << position.y << endl;
                    player.sprite.move(0, -5);
                }
                if (event.key.code == sf::Keyboard::A)
                {
                    sf::Vector2f position = player.sprite.getPosition();
                    //cout << "AAA: x: " << position.x << ", y: " << position.y << endl;
                    player.sprite.move(-5, 0);
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    sf::Vector2f position = player.sprite.getPosition();
                    //cout << "SSS: x: " << position.x << ", y: " << position.y << endl;
                    player.sprite.move(0, 5);
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    sf::Vector2f position = player.sprite.getPosition();
                    //cout << "DDD: x: " << position.x << ", y: " << position.y << endl;
                    player.sprite.move(5, 0);
                }
                break;

            default:
                break;
            }

            renderWindow.clear();
            draw();
            renderWindow.display();
        }
    }
}

void Game::draw()
{
    renderWindow.draw(player);
}