#pragma once

#include "pch.h"
#include "ResourceTypes.h"

struct Animation
{
public:
	Animation(sf::Sprite& sprite, res::Texture texture, float timePeriod, unsigned int frameSize = 32);

	void update();
	void onAnimationChange();

	void pause();
	void play();

private:
	bool getAnimationSwitch();

private:
	sf::Sprite& m_Sprite;
	sf::Texture m_Texture;
	sf::Clock m_Timer;
	float m_TimePeriod;
	unsigned int m_FrameSize;
	sf::IntRect m_CurrentFrame;
	bool m_Paused;
	bool m_AnimSwitch;

	unsigned int m_FrameNumber;
};