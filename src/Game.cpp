#include "Game.h"
#include <SFML/Window.hpp>
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
                player.move(event.key.code);
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