#pragma once

#include "pch.h"

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;

	SceneNode();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	// only draws current scene node, not the children
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const = 0; 

	void attachChild(Ptr child);
	SceneNode::Ptr detachChild(const SceneNode& node);

	void update(sf::Time dt);

	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;

private:
	virtual void updateCurrent(sf::Time dt) = 0;
	void updateChildren(sf::Time dt);

private:

	std::vector<Ptr> m_Children;
	SceneNode* m_Parent;
	
};