#pragma once
#include "pch.h"
#include "State.h"

class MenuState : public State
{
public:
	MenuState();


private:
	void render() override;
	void update(sf::Time deltaTime) override;
	void handleEvent(const sf::Event& event) override;
};