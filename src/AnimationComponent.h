#pragma once

#include "pch.h"
#include "Animation.h"

template <typename Identifier>
class AnimationComponent
{
public:
	AnimationComponent(sf::Sprite& sprite);

	void addAnimation(Identifier id, std::unique_ptr<Animation> animation);
	void update();

	void setCurrentAnimation(Identifier id);

	Identifier getCurrentAnimationID() { return m_CurrentAnimationID; }

	void pause() { 
	}

	void play()
	{
	}

private:
	sf::Sprite& m_Sprite;
	std::map<Identifier, std::unique_ptr<Animation>> m_Animations;
	Identifier m_CurrentAnimationID;
};

template <typename Identifier>
inline AnimationComponent<Identifier>::AnimationComponent(sf::Sprite& sprite) : m_Sprite(sprite)
{
}

template <typename Identifier>
inline void AnimationComponent<Identifier>::addAnimation(Identifier id, std::unique_ptr<Animation> animation)
{
	if (m_Animations.empty())
		m_CurrentAnimationID = id;
	m_Animations.try_emplace(id, std::move(animation));
}

template <typename Identifier>
inline void AnimationComponent<Identifier>::update()
{
	m_Animations[m_CurrentAnimationID]->update();
}

template <typename Identifier>
inline void AnimationComponent<Identifier>::setCurrentAnimation(Identifier id)
{
	auto found = m_Animations.find(id);
	if (found == m_Animations.end())
	{
		SPDLOG_ERROR("Cant find animation by id: " + std::to_string((int)id));
	}

	if (m_CurrentAnimationID != id)
	{
		SPDLOG_INFO("Animation changed: from" + std::to_string((int) m_CurrentAnimationID) + " to:" + std::to_string((int) id));
		m_CurrentAnimationID = id;
		m_Animations[m_CurrentAnimationID]->onAnimationChange();
	}
}
