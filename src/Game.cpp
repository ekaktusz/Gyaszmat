#include "Game.h"
#include <SFML/Window.hpp>

const std::string Game::name = "Gyaszmat";

Game::Game() : renderWindow({Game::XX, Game::YY}, Game::name)
{
    renderWindow.setFramerateLimit(60);
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
        update();
        render();
    }
}

void Game::update()
{
    while (renderWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            renderWindow.close();
        }
    }

    player.update();
}

void Game::render()
{
    renderWindow.clear();

    renderWindow.draw(player);

    renderWindow.display();
}