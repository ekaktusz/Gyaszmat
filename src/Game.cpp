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
    sf::Vector2f position, previous;
    const float speed = 60.0f;
    sf::Clock clock;
    float accumulator = 0;
    const float timestep = 1.0f / 10.0f;

    while (renderWindow.isOpen())
    {
        sf::Event event;
        while (renderWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                renderWindow.close();
            }
        }

        accumulator += clock.restart().asSeconds();
        while (accumulator >= timestep)
        {
            accumulator -= timestep;
            previous = position;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                position.x -= speed;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                position.x += speed;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && position.y >= 60)
                position.y -= speed;
            else if (position.y <= 0)
                position.y += speed;
        }

        renderWindow.clear();
        player.sprite.setPosition(previous + ((position - previous) * (accumulator / timestep)));
        draw();
        renderWindow.display();
    }
}

void Game::draw()
{
    renderWindow.draw(player);
}