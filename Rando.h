#pragma once
#include "Enemy.h"
class Rando :
	public Enemy
{
private:
	Direction GetNextDirection(Map& map) override;

public:
	using Enemy::Enemy;
};

