#pragma once

#include "pch.h"
#include "Animation.h"
 
// Component for Animations. Identifier should be an enum class from which the ID-s are gotten for the animation.
template <typename Identifier>
class AnimationComponent
{
public:
	AnimationComponent(sf::Sprite& sprite);

	void addAnimation(Identifier id, std::unique_ptr<Animation> animation);
	void update();

	void setCurrentAnimation(Identifier id);

	void pauseAnimation() { m_Animations[m_CurrentAnimationID]->pause(); }
	void playAnimation() { m_Animations[m_CurrentAnimationID]->play(); }

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
		m_CurrentAnimationID = id;
		m_Animations[m_CurrentAnimationID]->onAnimationChange();
	}
}
