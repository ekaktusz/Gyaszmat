#include "Animation.h"
#include "ResourceManager.h"

Animation::Animation(sf::Sprite& sprite, res::Texture texture, float timePeriod, unsigned int frameSize) :
	m_Sprite(sprite),
	m_TimePeriod(timePeriod),
	m_FrameSize(frameSize),
	m_Paused(false),
	m_AnimSwitch(true)
{

	m_Texture = ResourceManager::getInstance().getTexture(texture);

	m_CurrentFrame = sf::IntRect(0, 0, m_FrameSize, m_FrameSize);
	if (!m_Sprite.getTexture())
	{
		m_Sprite.setTexture(m_Texture);
		m_Sprite.setTextureRect(m_CurrentFrame);
	}
	m_FrameNumber = m_Texture.getSize().x / m_FrameSize;
}

void Animation::update()
{
	m_Sprite.setTexture(m_Texture);
	if (m_Timer.getElapsedTime().asSeconds() >= m_TimePeriod)
	{
		if (!m_Paused)
		{
			m_CurrentFrame.left += m_FrameSize;
			if (m_CurrentFrame.left >= (m_FrameNumber - 1) * m_FrameSize)
			{
				m_CurrentFrame.left = 0;
			}
			m_Timer.restart();
		}
		m_Sprite.setTextureRect(m_CurrentFrame);
	}
}

void Animation::onAnimationChange()
{
	//m_CurrentFrame = sf::IntRect(0, 0, m_FrameSize, m_FrameSize);
	//m_Sprite.setTextureRect(m_CurrentFrame);
	//m_Timer.restart();
}
