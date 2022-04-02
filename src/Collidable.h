#pragma once

class Player;

// TODO: For now only collision is with player, but change it to collidable in the future
class Collidable
{
public:
	virtual ~Collidable() {};
	virtual void updateCollision(Player&) = 0;
};