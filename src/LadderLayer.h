#pragma once

#include "pch.h"
#include "Collidable.h"

class ObjectLayer;
class Player;

class LadderLayer : public Collidable
{
public:
	LadderLayer(const tmx::Map* map);
	~LadderLayer();

	virtual void updateCollision(Player& player) override;

private:
	void updateCollisionBotLayer(Player& player);
	void updateCollisionMidLayer(Player& player);
	void updateCollisionTopLayer(Player& player);

private:
	ObjectLayer* m_BotLadderLayer;
	ObjectLayer* m_MidLadderLayer;
	ObjectLayer* m_TopLadderLayer;
};