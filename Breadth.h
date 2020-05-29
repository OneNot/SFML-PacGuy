#pragma once
#include "Enemy.h"
#include "Player.h"
#include <vector>
#include <queue>
#include <algorithm>

class Breadth :
	public Enemy
{
private:
	Direction GetNextDirection(Map& map) override;
	std::vector<sf::Vector2i> GetPathToPlayer(Map& map);
	std::vector<std::vector<sf::Vector2i>> solveBFS(Map& map);


public:
	using Enemy::Enemy;
};

