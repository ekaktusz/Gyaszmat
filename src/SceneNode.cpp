#include "SceneNode.h"


 SceneNode::SceneNode() : m_Children(), m_Parent(nullptr)
{}

void SceneNode::attachChild(Ptr child)
{
	child->m_Parent = this;
	m_Children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	// find given node in children
	auto found = std::find_if(
		m_Children.begin(), m_Children.end(), [&](Ptr& p) -> bool { return p.get() == &node; });

	if (found != m_Children.end())
	{
		SPDLOG_ERROR("Scene node can't be found in children.");
		return nullptr;
	}

	Ptr result = std::move(*found);
	result->m_Parent = nullptr;
	m_Children.erase(found);
	return result;
}

void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::updateChildren(sf::Time dt)
{
	for (Ptr& child : m_Children)
	{
		child->update(dt);
	}
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->m_Parent)
	{
		transform = node->getTransform() * transform;
	}

	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);

	for (const Ptr& child : m_Children)
	{
		child->draw(target, states);
	}
}