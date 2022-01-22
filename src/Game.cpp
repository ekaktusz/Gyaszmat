#include "Game.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const std::string Game::name = "Gyaszmat";

Game::Game(/* args */)
{
}

Game::~Game()
{
}

void Game::run()
{
    sf::RenderWindow window(sf::VideoMode(Game::XX, Game::YY), Game::name);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}