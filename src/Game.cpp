#include "Game.h"
#include <SFML/Window.hpp>

#include <iostream>

const std::string Game::name = "Gyaszmat";

Game::Game() : renderWindow({Game::XX, Game::YY}, Game::name)
{
    this->renderWindow.setFramerateLimit(60);
}

Game::~Game()
{
}

void Game::run()
{
    while (this->renderWindow.isOpen())
    {
        this->update();
        this->render();
    }
}

void Game::update()
{
    while (this->renderWindow.pollEvent(this->event))
    {
        if (this->event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            this->renderWindow.close();
        }
        if (this->event.type == sf::Event::KeyReleased &&
            (
            this->event.key.code == sf::Keyboard::A ||
            this->event.key.code == sf::Keyboard::D ||
            this->event.key.code == sf::Keyboard::W ||
            this->event.key.code == sf::Keyboard::S
            ))
        {
            this->player.resetAnimationTimer();
        }
        
    }

    this->player.update();
    this->updateCollision();
}

void Game::render()
{
    this->renderWindow.clear();

    this->renderWindow.draw(player);

    this->renderWindow.display();
}

void Game::updateCollision()
{
    if (this->player.getGlobalBounds().top + this->player.getGlobalBounds().height > this->renderWindow.getSize().y)
    {
        this->player.stopFalling();
        this->player.setPosition(
            this->player.getGlobalBounds().left,
            this->renderWindow.getSize().y - this->player.getGlobalBounds().height
        );
    }
}